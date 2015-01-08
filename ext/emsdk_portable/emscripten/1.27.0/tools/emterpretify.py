#!/usr/bin/env python2

'''
Processes asm.js code to make it run in an emterpreter.

Currently this requires the asm.js code to have been built with -s FINALIZE_ASM_JS=0
'''

import os, sys, re, json
import asm_module, shared, shutil

# params

INNERTERPRETER_LAST_OPCODE = 0 # 'CONDD'

EMT_STACK_MAX = 1024*1024

LOG_CODE = os.environ.get('EMCC_LOG_EMTERPRETER_CODE')

# consts

BLACKLIST = set(['_malloc', '_free', '_memcpy', '_memmove', '_memset', 'copyTempDouble', 'copyTempFloat', '_strlen', 'stackAlloc', 'setThrew', 'stackRestore', 'setTempRet0', 'getTempRet0', 'stackSave', 'runPostSets', '_emscripten_autodebug_double', '_emscripten_autodebug_float', '_emscripten_autodebug_i8', '_emscripten_autodebug_i16', '_emscripten_autodebug_i32', '_emscripten_autodebug_i64', '_strncpy', '_strcpy', '_strcat', '_saveSetjmp', '_testSetjmp', '_emscripten_replace_memory', '_bitshift64Shl', '_bitshift64Ashr', '_bitshift64Lshr'])

OPCODES = [ # l, lx, ly etc - one of 256 locals
  'SET',     # [lx, ly, 0]          lx = ly (int or float, not double)
  'SETVI',   # [l, vl, vh]          l = v (16-bit signed int)
  'SETVIB',  # [l, 0, 0] [..v..]    l = 32-bit int in next 32-bit instruction

  'ADD',     # [lx, ly, lz]         lx = ly + lz (32-bit int)
  'SUB',     # [lx, ly, lz]         lx = ly - lz (32-bit int)
  'MUL',     # [lx, ly, lz]         lx = ly * lz (32-bit int)
  'SDIV',    # [lx, ly, lz]         lx = ly / lz (32-bit signed int)
  'UDIV',    # [lx, ly, lz]         lx = ly / lz (32-bit unsigned int)
  'SMOD',    # [lx, ly, lz]         lx = ly % lz (32-bit signed int)
  'UMOD',    # [lx, ly, lz]         lx = ly % lz (32-bit unsigned int)
  'NEG',     # [lx, ly, 0]          lx = -ly (int)
  'BNOT',    # [lx, ly, 0]          ly = ~ly (int)

  'LNOT',    # [lx, ly, 0]          ly = !ly (int)
  'EQ',      # [lx, ly, lz]         lx = ly == lz (32-bit int)
  'NE',      # [lx, ly, lz]         lx = ly != lz (32-bit int)
  'SLT',     # [lx, ly, lz]         lx = ly < lz (32-bit signed)
  'ULT',     # [lx, ly, lz]         lx = ly < lz (32-bit unsigned)
  'SLE',     # [lx, ly, lz]         lx = ly <= lz (32-bit signed)
  'ULE',     # [lx, ly, lz]         lx = ly <= lz (32-bit unsigned)

  'AND',     # [lx, ly, lz]         lx = ly & lz
  'OR',      # [lx, ly, lz]         lx = ly | lz
  'XOR',     # [lx, ly, lz]         lx = ly ^ lz
  'SHL',     # [lx, ly, lz]         lx = ly << lz
  'ASHR',    # [lx, ly, lz]         lx = ly >> lz
  'LSHR',    # [lx, ly, lz]         lx = ly >>> lz

  'ADDV',    # [lx, ly, v]          lx = ly + v (32-bit int, v is 8-bit signed)
  'SUBV',
  'MULV',
  'SDIVV',
  'UDIVV',    #                                              (v is 8-bit unsigned)
  'SMODV',
  'UMODV',    #                                              (v is 8-bit unsigned)
  'EQV',
  'NEV',
  'SLTV',
  'ULTV',    #                                               (v is 8-bit unsigned)
  'SLEV',
  'ULEV',    #                                               (v is 8-bit unsigned)
  'ANDV',
  'ORV',
  'XORV',
  'SHLV',    #                                               (v is 8-bit unsigned)
  'ASHRV',   #                                               (v is 8-bit unsigned)
  'LSHRV',   #                                               (v is 8-bit unsigned)

  'LNOTBRF',    # [cond] [absolute-target]      cond+branch
  'EQBRF',
  'NEBRF',
  'SLTBRF',
  'ULTBRF',
  'SLEBRF',
  'ULEBRF',
  'LNOTBRT',
  'EQBRT',
  'NEBRT',
  'SLTBRT',
  'ULTBRT',
  'SLEBRT',
  'ULEBRT',

  'SETD',    # [lx, ly, lz]         lx = ly (double)
  'SETVD',   # [lx, vl, vh]         lx = ly (16 bit signed int, converted into double)
  'SETVDI',  # [lx, 0, 0] [..v..]   lx = v (32 bit signed int, converted into double)
  'SETVDF',  # [lx, 0, 0] [..v..]   lx = v (32 bit float, converted into double)
  'SETVDD',  # [lx, 0, 0][.v.][.v.] lx = v (64 bit double)
  'ADDD',    # [lx, ly, lz]         lx = ly + lz (double)
  'SUBD',    # [lx, ly, lz]         lx = ly - lz (double)
  'MULD',    # [lx, ly, lz]         lx = ly * lz (double)
  'DIVD',    # [lx, ly, lz]         lx = ly / lz (double)
  'MODD',    # [lx, ly, lz]         lx = ly % lz (double)
  'NEGD',    # [lx, ly, 0]          lx = -ly (double)
  'EQD',     # [lx, ly, lz]         lx = ly == lz (double)
  'NED',     # [lx, ly, lz]         lx = ly != lz (double)
  'LTD',     # [lx, ly, lz]         lx = ly < lz (signed)
  'LED',     # [lx, ly, lz]         lx = ly < lz (double)
  'GTD',     # [lx, ly, lz]         lx = ly <= lz (double)
  'GED',     # [lx, ly, lz]         lx = ly <= lz (double)
  'D2I',     # [lx, ly, 0]          lx = ~~ly (double-to-int)
  'SI2D',    # [lx, ly, 0]          lx = +ly (signed int-to-double)
  'UI2D',    # [lx, ly, 0]          lx = +ly (unsigned int-to-double)

  'LOAD8',   # [lx, ly, 0]          lx = HEAP8[ly >> 0]
  'LOADU8',  # [lx, ly, 0]          lx = HEAPU8[ly >> 0]
  'LOAD16',  # [lx, ly, 0]          lx = HEAP16[ly >> 1]
  'LOADU16', # [lx, ly, 0]          lx = HEAPU16[ly >> 1]
  'LOAD32',  # [lx, ly, 0]          lx = HEAP32[ly >> 2]   - no need for unsigned version, this is set to a register anyhow
  'STORE8',  # [lx, ly, 0]          HEAP8[lx >> 2] = ly
  'STORE16', # [lx, ly, 0]          HEAP16[lx >> 2] = ly
  'STORE32', # [lx, ly, 0]          HEAP32[lx >> 2] = ly
  'LOADF64',  # [lx, ly, 0]         lx = HEAPF64[ly >> 3]
  'STOREF64', # [lx, ly, 0]         HEAPF64[lx >> 3] = ly
  'LOADF32',  # [lx, ly, 0]         lx = HEAPF32[ly >> 3]
  'STOREF32', # [lx, ly, 0]         HEAPF32[lx >> 3] = ly

  'LOAD8A',   # [lx, ly, lz]        load-add and store-add instructions, whose pointer input is a signed addition: lx = load(ly + lz), store(lx + ly) = lz
  'LOADU8A',
  'LOAD16A',
  'LOADU16A',
  'LOAD32A',
  'STORE8A',
  'STORE16A',
  'STORE32A',
  'LOADF64A',
  'STOREF64A',
  'LOADF32A',
  'STOREF32A',

  'LOAD8AV',   # [lx, ly, lz]        load-add and store-add instructions, whose pointer input is a signed addition: lx = load(ly + lz), store(lx + ly) = lz, where the second add op is 8-bit signed
  'LOADU8AV',
  'LOAD16AV',
  'LOADU16AV',
  'LOAD32AV',
  'STORE8AV',
  'STORE16AV',
  'STORE32AV',
  'LOADF64AV',
  'STOREF64AV',
  'LOADF32AV',
  'STOREF32AV',

  'STORE8C',
  'STORE16C',
  'STORE32C',
  'STOREF64C',
  'STOREF32C',

  'BR',      # [0, tl, th]          jump t instructions (multiple of 4)
  'BRT',     # [cond, tl, th]       if cond, jump t instructions (multiple of 4)
  'BRF',     # [cond, tl, th]       if !cond, jump t instructions (multiple of 4)
  'BRA',      # [0, 0, 0] [addr]     jump to addr
  'BRTA',     # [cond, 0, 0] [addr]  if cond, jump to addr
  'BRFA',     # [cond, 0, 0] [addr]  if !cond, jump to addr

  'COND',    # [out, cond, x] [y]   out = cond ? x : y, int
  'CONDD',   # [out, cond, x] [y]   out = cond ? x : y, double

  'GETTDP',  # [l, 0, 0]            l = tempDoublePtr
  'GETTR0',  # [l, 0, 0]            l = tempRet0
  'SETTR0',  # [l, 0, 0]            tempRet0 = l
  'GETGLBI', # [l, vl, vh]          get global value, int, indexed by v
  #'GETGLBD', # [l, vl, vh]          get global value, double, indexed by v
  'SETGLBI', # [vl, vh, l]          set global value, int, indexed by v (v = l)

  'INTCALL', # [lx, 0, 0] [target] [params]         (lx = ) target(params..)
                    #                               Internal, emterpreter-to-emterpreter call.
  'EXTCALL', # [lx, targetl, targeth] [params...]   (lx = ) target(params..) lx's existence and type depend on the target's actual callsig;
                    #                                 this instruction can take multiple 32-bit instruction chunks
                    #                                 if target is a function table, then the first param is the index of the register holding the function pointer

  'GETST',   # [l, 0, 0]            l = STACKTOP
  'SETST',   # [l, 0, 0]            STACKTOP = l

  'SWITCH',  # [lx, ly, lz]         switch (lx) { .. }. followed by a jump table for values in range [ly..ly+lz), after which is the default (which might be empty)
  'RET',     # [l, 0, 0]            return l (depending on which emterpreter_x we are in, has the right type)
  'FUNC',    # [total locals, num params, which emterpreter (0 = normal, 1 = zero)] [num params + num zero-inits, 0, 0, 0]           function with n locals (each taking 64 bits), of which the first are params
]

def randomize_opcodes():
  global OPCODES
  import random
  random.shuffle(opcodes)
  print OPCODES
#randomize_opcodes()

assert len(OPCODES) == len(set(OPCODES)) # no dupe names
assert len(OPCODES) < 256

ROPCODES = {}
for i in range(len(OPCODES)):
  ROPCODES[OPCODES[i]] = i

GLOBAL_BASE = 256*8

# utils

settings = { 'PRECISE_F32': 0 } # TODO

def bytify(x):
  assert x >= 0 and x < (1 << 32)
  return [x & 255, (x >> 8) & 255, (x >> 16) & 255, (x >> 24) & 255]

def next_power_of_two(x):
  if x == 0: return 0
  ret = 1
  while ret < x: ret <<= 1
  return ret

def get_access(l, s='i', base='sp', offset=None):
  if offset is not None:
    offset = '+ ' + str(offset) + ' '
  else:
    offset = ''
  if s == 'i':
    return 'HEAP32[' + str(base) + ' + (' + l + ' << 3) ' + offset + '>> 2]'
  elif s == 'd':
    return 'HEAPF64[' + str(base) + ' + (' + l + ' << 3) ' + offset + '>> 3]'
  else:
    assert 0

def get_coerced_access(l, s='i', unsigned=False, base='sp', offset=None):
  if s == 'i':
    if not unsigned:
      return get_access(l, s, base, offset) + '|0'
    else:
      return get_access(l, s, base, offset) + '>>>0'
  elif s == 'd':
    return '+' + get_access(l, s, base, offset)
  else:
    assert 0

CASES = {}
CASES[ROPCODES['SET']] = get_access('lx') + ' = ' + get_coerced_access('ly') + ';'
CASES[ROPCODES['GETST']] = get_access('lx') + ' = STACKTOP;'
CASES[ROPCODES['SETST']] = 'STACKTOP = ' + get_coerced_access('lx') + ';'
CASES[ROPCODES['SETVI']] = get_access('lx') + ' = inst >> 16;'
CASES[ROPCODES['SETVIB']] = 'pc = pc + 4 | 0; ' + get_access('lx') + ' = HEAP32[pc >> 2] | 0;'

CASES[ROPCODES['ADD']] = get_access('lx') + ' = (' + get_coerced_access('ly') + ') + (' + get_coerced_access('lz') + ') | 0;'
CASES[ROPCODES['SUB']] = get_access('lx') + ' = (' + get_coerced_access('ly') + ') - (' + get_coerced_access('lz') + ') | 0;'
CASES[ROPCODES['MUL']] = get_access('lx') + ' = Math_imul(' + get_coerced_access('ly') + ', ' + get_coerced_access('lz') + ') | 0;'
CASES[ROPCODES['SDIV']] = get_access('lx') + ' = (' + get_coerced_access('ly') + ') / (' + get_coerced_access('lz') + ') | 0;'
CASES[ROPCODES['UDIV']] = get_access('lx') + ' = (' + get_coerced_access('ly', unsigned=True) + ') / (' + get_coerced_access('lz', unsigned=True) + ') >>> 0;'
CASES[ROPCODES['SMOD']] = get_access('lx') + ' = (' + get_coerced_access('ly') + ') % (' + get_coerced_access('lz') + ') | 0;'
CASES[ROPCODES['UMOD']] = get_access('lx') + ' = (' + get_coerced_access('ly', unsigned=True) + ') % (' + get_coerced_access('lz', unsigned=True) + ') >>> 0;'
CASES[ROPCODES['NEG']] = get_access('lx') + ' = -(' + get_coerced_access('ly') + ');'
CASES[ROPCODES['BNOT']] = get_access('lx') + ' = ~(' + get_coerced_access('ly') + ');'

CASES[ROPCODES['LNOT']] = get_access('lx') + ' = !(' + get_coerced_access('ly') + ');'
CASES[ROPCODES['EQ']] = get_access('lx') + ' = (' + get_coerced_access('ly') + ') == (' + get_coerced_access('lz') + ') | 0;'
CASES[ROPCODES['NE']] = get_access('lx') + ' = (' + get_coerced_access('ly') + ') != (' + get_coerced_access('lz') + ') | 0;'
CASES[ROPCODES['SLT']] = get_access('lx') + ' = (' + get_coerced_access('ly') + ') < (' + get_coerced_access('lz') + ') | 0;'
CASES[ROPCODES['ULT']] = get_access('lx') + ' = (' + get_coerced_access('ly', unsigned=True) + ') < (' + get_coerced_access('lz', unsigned=True) + ') | 0;'
CASES[ROPCODES['SLE']] = get_access('lx') + ' = (' + get_coerced_access('ly') + ') <= (' + get_coerced_access('lz') + ') | 0;'
CASES[ROPCODES['ULE']] = get_access('lx') + ' = (' + get_coerced_access('ly', unsigned=True) + ') <= (' + get_coerced_access('lz', unsigned=True) + ') | 0;'

CASES[ROPCODES['AND']] = get_access('lx') + ' = (' + get_coerced_access('ly') + ') & (' + get_coerced_access('lz') + ');'
CASES[ROPCODES['OR']] = get_access('lx') + ' = (' + get_coerced_access('ly') + ') | (' + get_coerced_access('lz') + ');'
CASES[ROPCODES['XOR']] = get_access('lx') + ' = (' + get_coerced_access('ly') + ') ^ (' + get_coerced_access('lz') + ');'
CASES[ROPCODES['SHL']] = get_access('lx') + ' = (' + get_coerced_access('ly') + ') << (' + get_coerced_access('lz') + ');'
CASES[ROPCODES['ASHR']] = get_access('lx') + ' = (' + get_coerced_access('ly') + ') >> (' + get_coerced_access('lz') + ');'
CASES[ROPCODES['LSHR']] = get_access('lx') + ' = (' + get_coerced_access('ly') + ') >>> (' + get_coerced_access('lz') + ');'

CASES[ROPCODES['ADDV']] = get_access('lx') + ' = (' + get_coerced_access('ly') + ') + (inst >> 24) | 0;'
CASES[ROPCODES['SUBV']] = get_access('lx') + ' = (' + get_coerced_access('ly') + ') - (inst >> 24) | 0;'
CASES[ROPCODES['MULV']] = get_access('lx') + ' = Math_imul(' + get_coerced_access('ly') + ', inst >> 24) | 0;'
CASES[ROPCODES['SDIVV']] = get_access('lx') + ' = (' + get_coerced_access('ly') + ') / (inst >> 24) | 0;'
CASES[ROPCODES['UDIVV']] = get_access('lx') + ' = (' + get_coerced_access('ly', unsigned=True) + ') / (lz >>> 0) >>> 0;'
CASES[ROPCODES['SMODV']] = get_access('lx') + ' = (' + get_coerced_access('ly') + ') % (inst >> 24) | 0;'
CASES[ROPCODES['UMODV']] = get_access('lx') + ' = (' + get_coerced_access('ly', unsigned=True) + ') % (lz >>> 0) >>> 0;'
CASES[ROPCODES['EQV']] = get_access('lx') + ' = (' + get_coerced_access('ly') + ') == (inst >> 24) | 0;'
CASES[ROPCODES['NEV']] = get_access('lx') + ' = (' + get_coerced_access('ly') + ') != (inst >> 24) | 0;'
CASES[ROPCODES['SLTV']] = get_access('lx') + ' = (' + get_coerced_access('ly') + ') < (inst >> 24) | 0;'
CASES[ROPCODES['ULTV']] = get_access('lx') + ' = (' + get_coerced_access('ly', unsigned=True) + ') < (lz >>> 0) | 0;'
CASES[ROPCODES['SLEV']] = get_access('lx') + ' = (' + get_coerced_access('ly') + ') <= (inst >> 24) | 0;'
CASES[ROPCODES['ULEV']] = get_access('lx') + ' = (' + get_coerced_access('ly', unsigned=True) + ') <= (lz >>> 0) | 0;'
CASES[ROPCODES['ANDV']] = get_access('lx') + ' = (' + get_coerced_access('ly') + ') & (inst >> 24);'
CASES[ROPCODES['ORV']] = get_access('lx') + ' = (' + get_coerced_access('ly') + ') | (inst >> 24);'
CASES[ROPCODES['XORV']] = get_access('lx') + ' = (' + get_coerced_access('ly') + ') ^ (inst >> 24);'
CASES[ROPCODES['SHLV']] = get_access('lx') + ' = (' + get_coerced_access('ly') + ') << lz;'
CASES[ROPCODES['ASHRV']] = get_access('lx') + ' = (' + get_coerced_access('ly') + ') >> lz;'
CASES[ROPCODES['LSHRV']] = get_access('lx') + ' = (' + get_coerced_access('ly') + ') >>> lz;'

CASES[ROPCODES['LNOTBRF']] = 'if (' + get_coerced_access('ly') + ') { pc = HEAP32[pc + 4 >> 2] | 0; PROCEED_WITHOUT_PC_BUMP; } else { pc = pc + 4 | 0; }'
CASES[ROPCODES['EQBRF']] = 'if ((' + get_coerced_access('ly') + ') == (' + get_coerced_access('lz') + ')) { pc = pc + 4 | 0; } else { pc = HEAP32[pc + 4 >> 2] | 0; PROCEED_WITHOUT_PC_BUMP; }'
CASES[ROPCODES['NEBRF']] = 'if ((' + get_coerced_access('ly') + ') != (' + get_coerced_access('lz') + ')) { pc = pc + 4 | 0; } else { pc = HEAP32[pc + 4 >> 2] | 0; PROCEED_WITHOUT_PC_BUMP; }'
CASES[ROPCODES['SLTBRF']] = 'if ((' + get_coerced_access('ly') + ') < (' + get_coerced_access('lz') + ')) { pc = pc + 4 | 0; } else { pc = HEAP32[pc + 4 >> 2] | 0; PROCEED_WITHOUT_PC_BUMP; }'
CASES[ROPCODES['ULTBRF']] = 'if ((' + get_coerced_access('ly', unsigned=True) + ') < (' + get_coerced_access('lz', unsigned=True) + ')) { pc = pc + 4 | 0; } else { pc = HEAP32[pc + 4 >> 2] | 0; PROCEED_WITHOUT_PC_BUMP; }'
CASES[ROPCODES['SLEBRF']] = 'if ((' + get_coerced_access('ly') + ') <= (' + get_coerced_access('lz') + ')) { pc = pc + 4 | 0; } else { pc = HEAP32[pc + 4 >> 2] | 0; PROCEED_WITHOUT_PC_BUMP; }'
CASES[ROPCODES['ULEBRF']] = 'if ((' + get_coerced_access('ly', unsigned=True) + ') <= (' + get_coerced_access('lz', unsigned=True) + ')) { pc = pc + 4 | 0; } else { pc = HEAP32[pc + 4 >> 2] | 0; PROCEED_WITHOUT_PC_BUMP; }'

CASES[ROPCODES['LNOTBRT']] = 'if (' + get_coerced_access('ly') + ') { pc = pc + 4 | 0; } else { pc = HEAP32[pc + 4 >> 2] | 0; PROCEED_WITHOUT_PC_BUMP; }'
CASES[ROPCODES['EQBRT']] = 'if ((' + get_coerced_access('ly') + ') == (' + get_coerced_access('lz') + ')) { pc = HEAP32[pc + 4 >> 2] | 0; PROCEED_WITHOUT_PC_BUMP; } else { pc = pc + 4 | 0; }'
CASES[ROPCODES['NEBRT']] = 'if ((' + get_coerced_access('ly') + ') != (' + get_coerced_access('lz') + ')) { pc = HEAP32[pc + 4 >> 2] | 0; PROCEED_WITHOUT_PC_BUMP; } else { pc = pc + 4 | 0; }'
CASES[ROPCODES['SLTBRT']] = 'if ((' + get_coerced_access('ly') + ') < (' + get_coerced_access('lz') + ')) { pc = HEAP32[pc + 4 >> 2] | 0; PROCEED_WITHOUT_PC_BUMP; } else { pc = pc + 4 | 0; }'
CASES[ROPCODES['ULTBRT']] = 'if ((' + get_coerced_access('ly', unsigned=True) + ') < (' + get_coerced_access('lz', unsigned=True) + ')) { pc = HEAP32[pc + 4 >> 2] | 0; PROCEED_WITHOUT_PC_BUMP; } else { pc = pc + 4 | 0; }'
CASES[ROPCODES['SLEBRT']] = 'if ((' + get_coerced_access('ly') + ') <= (' + get_coerced_access('lz') + ')) { pc = HEAP32[pc + 4 >> 2] | 0; PROCEED_WITHOUT_PC_BUMP; } else { pc = pc + 4 | 0; }'
CASES[ROPCODES['ULEBRT']] = 'if ((' + get_coerced_access('ly', unsigned=True) + ') <= (' + get_coerced_access('lz', unsigned=True) + ')) { pc = HEAP32[pc + 4 >> 2] | 0; PROCEED_WITHOUT_PC_BUMP; } else { pc = pc + 4 | 0; }'

CASES[ROPCODES['SETD']] = get_access('lx', s='d') + ' = ' + get_coerced_access('ly', s='d') + ';'
CASES[ROPCODES['SETVD']] = get_access('lx', s='d') + ' = +(inst >> 16);'
CASES[ROPCODES['SETVDI']] = 'pc = pc + 4 | 0; ' + get_access('lx', s='d') + ' = +(HEAP32[pc >> 2] | 0);'
CASES[ROPCODES['SETVDF']] = 'pc = pc + 4 | 0; ' + get_access('lx', s='d') + ' = +HEAPF32[pc >> 2];'
CASES[ROPCODES['SETVDD']] = 'HEAP32[tempDoublePtr >> 2] = HEAP32[pc + 4 >> 2]; HEAP32[tempDoublePtr + 4 >> 2] = HEAP32[pc + 8 >> 2]; pc = pc + 8 | 0; ' + get_access('lx', s='d') + ' = +HEAPF64[tempDoublePtr >> 3];'
CASES[ROPCODES['ADDD']] = get_access('lx', s='d') + ' = (' + get_coerced_access('ly', s='d') + ') + (' + get_coerced_access('lz', s='d') + ');'
CASES[ROPCODES['SUBD']] = get_access('lx', s='d') + ' = (' + get_coerced_access('ly', s='d') + ') - (' + get_coerced_access('lz', s='d') + ');'
CASES[ROPCODES['MULD']] = get_access('lx', s='d') + ' = (' + get_coerced_access('ly', s='d') + ') * (' + get_coerced_access('lz', s='d') + ');'
CASES[ROPCODES['DIVD']] = get_access('lx', s='d') + ' = (' + get_coerced_access('ly', s='d') + ') / (' + get_coerced_access('lz', s='d') + ');'
CASES[ROPCODES['MODD']] = get_access('lx', s='d') + ' = (' + get_coerced_access('ly', s='d') + ') % (' + get_coerced_access('lz', s='d') + ');'
CASES[ROPCODES['NEGD']] = get_access('lx', s='d') + ' = -(' + get_coerced_access('ly', s='d') + ');'
CASES[ROPCODES['EQD']] = get_access('lx') + ' = (' + get_coerced_access('ly', s='d') + ') == (' + get_coerced_access('lz', s='d') + ') | 0;'
CASES[ROPCODES['NED']] = get_access('lx') + ' = (' + get_coerced_access('ly', s='d') + ') != (' + get_coerced_access('lz', s='d') + ') | 0;'
CASES[ROPCODES['LTD']] = get_access('lx') + ' = (' + get_coerced_access('ly', s='d') + ') < (' + get_coerced_access('lz', s='d') + ') | 0;'
CASES[ROPCODES['LED']] = get_access('lx') + ' = (' + get_coerced_access('ly', s='d') + ') <= (' + get_coerced_access('lz', s='d') + ') | 0;'
CASES[ROPCODES['GTD']] = get_access('lx') + ' = (' + get_coerced_access('ly', s='d') + ') > (' + get_coerced_access('lz', s='d') + ') | 0;'
CASES[ROPCODES['GED']] = get_access('lx') + ' = (' + get_coerced_access('ly', s='d') + ') >= (' + get_coerced_access('lz', s='d') + ') | 0;'
CASES[ROPCODES['D2I']] = get_access('lx') + ' = ~~(' + get_coerced_access('ly', s='d') + ');'
CASES[ROPCODES['SI2D']] = get_access('lx', s='d') + ' = +(' + get_coerced_access('ly') + ');'
CASES[ROPCODES['UI2D']] = get_access('lx', s='d') + ' = +(' + get_coerced_access('ly', unsigned=True) + ');'

CASES[ROPCODES['LOAD8']] = get_access('lx') + ' = ' + 'HEAP8[' + get_access('ly') + ' >> 0];'
CASES[ROPCODES['LOADU8']] = get_access('lx') + ' = ' + 'HEAPU8[' + get_access('ly') + ' >> 0];'
CASES[ROPCODES['LOAD16']] = get_access('lx') + ' = ' + 'HEAP16[' + get_access('ly') + ' >> 1];'
CASES[ROPCODES['LOADU16']] = get_access('lx') + ' = ' + 'HEAPU16[' + get_access('ly') + ' >> 1];'
CASES[ROPCODES['LOAD32']] = get_access('lx') + ' = ' + 'HEAP32[' + get_access('ly') + ' >> 2];'
CASES[ROPCODES['STORE8']] = 'HEAP8[' + get_access('lx') + ' >> 0] = ' + get_coerced_access('ly') + ';'
CASES[ROPCODES['STORE16']] = 'HEAP16[' + get_access('lx') + ' >> 1] = ' + get_coerced_access('ly') + ';'
CASES[ROPCODES['STORE32']] = 'HEAP32[' + get_access('lx') + ' >> 2] = ' + get_coerced_access('ly') + ';'
CASES[ROPCODES['LOADF64']] = get_access('lx', s='d') + ' = ' + '+HEAPF64[' + get_access('ly') + ' >> 3];'
CASES[ROPCODES['STOREF64']] = 'HEAPF64[' + get_access('lx') + ' >> 3] = ' + get_coerced_access('ly', s='d') + ';'
CASES[ROPCODES['LOADF32']] = get_access('lx', s='d') + ' = ' + '+HEAPF32[' + get_access('ly') + ' >> 2];'
CASES[ROPCODES['STOREF32']] = 'HEAPF32[' + get_access('lx') + ' >> 2] = ' + get_coerced_access('ly', s='d') + ';'

CASES[ROPCODES['LOAD8A']] = get_access('lx') + ' = ' + 'HEAP8[(' + get_coerced_access('ly') + ') + (' + get_coerced_access('lz') + ') >> 0];'
CASES[ROPCODES['LOADU8A']] = get_access('lx') + ' = ' + 'HEAPU8[(' + get_coerced_access('ly') + ') + (' + get_coerced_access('lz') + ') >> 0];'
CASES[ROPCODES['LOAD16A']] = get_access('lx') + ' = ' + 'HEAP16[(' + get_coerced_access('ly') + ') + (' + get_coerced_access('lz') + ') >> 1];'
CASES[ROPCODES['LOADU16A']] = get_access('lx') + ' = ' + 'HEAPU16[(' + get_coerced_access('ly') + ') + (' + get_coerced_access('lz') + ') >> 1];'
CASES[ROPCODES['LOAD32A']] = get_access('lx') + ' = ' + 'HEAP32[(' + get_coerced_access('ly') + ') + (' + get_coerced_access('lz') + ') >> 2];'
CASES[ROPCODES['STORE8A']] = 'HEAP8[(' + get_coerced_access('lx') + ') + (' + get_coerced_access('ly') + ') >> 0] = ' + get_coerced_access('lz') + ';'
CASES[ROPCODES['STORE16A']] = 'HEAP16[(' + get_coerced_access('lx') + ') + (' + get_coerced_access('ly') + ') >> 1] = ' + get_coerced_access('lz') + ';'
CASES[ROPCODES['STORE32A']] = 'HEAP32[(' + get_coerced_access('lx') + ') + (' + get_coerced_access('ly') + ') >> 2] = ' + get_coerced_access('lz') + ';'
CASES[ROPCODES['LOADF64A']] = get_access('lx', s='d') + ' = ' + '+HEAPF64[(' + get_coerced_access('ly') + ') + (' + get_coerced_access('lz') + ') >> 3];'
CASES[ROPCODES['STOREF64A']] = 'HEAPF64[(' + get_coerced_access('lx') + ') + (' + get_coerced_access('ly') + ') >> 3] = ' + get_coerced_access('lz', s='d') + ';'
CASES[ROPCODES['LOADF32A']] = get_access('lx', s='d') + ' = ' + '+HEAPF32[(' + get_coerced_access('ly') + ') + (' + get_coerced_access('lz') + ') >> 2];'
CASES[ROPCODES['STOREF32A']] = 'HEAPF32[(' + get_coerced_access('lx') + ') + (' + get_coerced_access('ly') + ') >> 2] = ' + get_coerced_access('lz', s='d') + ';'

CASES[ROPCODES['LOAD8AV']] = get_access('lx') + ' = ' + 'HEAP8[(' + get_coerced_access('ly') + ') + (inst >> 24) >> 0];'
CASES[ROPCODES['LOADU8AV']] = get_access('lx') + ' = ' + 'HEAPU8[(' + get_coerced_access('ly') + ') + (inst >> 24) >> 0];'
CASES[ROPCODES['LOAD16AV']] = get_access('lx') + ' = ' + 'HEAP16[(' + get_coerced_access('ly') + ') + (inst >> 24) >> 1];'
CASES[ROPCODES['LOADU16AV']] = get_access('lx') + ' = ' + 'HEAPU16[(' + get_coerced_access('ly') + ') + (inst >> 24) >> 1];'
CASES[ROPCODES['LOAD32AV']] = get_access('lx') + ' = ' + 'HEAP32[(' + get_coerced_access('ly') + ') + (inst >> 24) >> 2];'
CASES[ROPCODES['STORE8AV']] = 'HEAP8[(' + get_coerced_access('lx') + ') + (ly << 24 >> 24) >> 0] = ' + get_coerced_access('lz') + ';'
CASES[ROPCODES['STORE16AV']] = 'HEAP16[(' + get_coerced_access('lx') + ') + (ly << 24 >> 24) >> 1] = ' + get_coerced_access('lz') + ';'
CASES[ROPCODES['STORE32AV']] = 'HEAP32[(' + get_coerced_access('lx') + ') + (ly << 24 >> 24) >> 2] = ' + get_coerced_access('lz') + ';'
CASES[ROPCODES['LOADF64AV']] = get_access('lx', s='d') + ' = ' + '+HEAPF64[(' + get_coerced_access('ly') + ') + (inst >> 24) >> 3];'
CASES[ROPCODES['STOREF64AV']] = 'HEAPF64[(' + get_coerced_access('lx') + ') + (ly << 24 >> 24) >> 3] = ' + get_coerced_access('lz', s='d') + ';'
CASES[ROPCODES['LOADF32AV']] = get_access('lx', s='d') + ' = ' + '+HEAPF32[(' + get_coerced_access('ly') + ') + (inst >> 24) >> 2];'
CASES[ROPCODES['STOREF32AV']] = 'HEAPF32[(' + get_coerced_access('lx') + ') + (ly << 24 >> 24) >> 2] = ' + get_coerced_access('lz', s='d') + ';'

CASES[ROPCODES['STORE8C']] = 'HEAP8[' + get_access('lx') + ' >> 0] = HEAP8[' + get_access('ly') + ' >> 0] | 0;'
CASES[ROPCODES['STORE16C']] = 'HEAP16[' + get_access('lx') + ' >> 1] = HEAP16[' + get_access('ly') + ' >> 1] | 0;'
CASES[ROPCODES['STORE32C']] = 'HEAP32[' + get_access('lx') + ' >> 2] = HEAP32[' + get_access('ly') + ' >> 2] | 0;'
CASES[ROPCODES['STOREF32C']] = 'HEAPF32[' + get_access('lx') + ' >> 2] = +HEAPF32[' + get_access('ly') + ' >> 2];'
CASES[ROPCODES['STOREF64C']] = 'HEAPF64[' + get_access('lx') + ' >> 3] = +HEAPF64[' + get_access('ly') + ' >> 3];'

CASES[ROPCODES['BR']] = 'pc = pc + ((inst >> 16) << 2) | 0; PROCEED_WITHOUT_PC_BUMP;'
CASES[ROPCODES['BRT']] = 'if (' + get_coerced_access('lx') + ') { pc = pc + ((inst >> 16) << 2) | 0; PROCEED_WITHOUT_PC_BUMP; }'
CASES[ROPCODES['BRF']] = 'if (!(' + get_coerced_access('lx') + ')) { pc = pc + ((inst >> 16) << 2) | 0; PROCEED_WITHOUT_PC_BUMP; }'
CASES[ROPCODES['BRA']] = 'pc = HEAP32[pc + 4 >> 2] | 0; PROCEED_WITHOUT_PC_BUMP;'
CASES[ROPCODES['BRTA']] = 'pc = pc + 4 | 0; if (' + get_coerced_access('lx') + ') { pc = HEAP32[pc >> 2] | 0; PROCEED_WITHOUT_PC_BUMP; }'
CASES[ROPCODES['BRFA']] = 'pc = pc + 4 | 0; if (!(' + get_coerced_access('lx') + ')) { pc = HEAP32[pc >> 2] | 0; PROCEED_WITHOUT_PC_BUMP; }'

CASES[ROPCODES['COND']] = 'pc = pc + 4 | 0; ' + get_access('lx') + ' = (' + get_coerced_access('ly') + ') ? (' + get_coerced_access('lz') + ') : (' + get_coerced_access('(HEAPU8[pc >> 0] | 0)') + ');'
CASES[ROPCODES['CONDD']] = 'pc = pc + 4 | 0; ' + get_access('lx', s='d') + ' = (' + get_coerced_access('ly') + ') ? (' + get_coerced_access('lz', s='d') + ') : (' + get_coerced_access('(HEAPU8[pc >> 0] | 0)', s='d') + ');'

CASES[ROPCODES['GETTDP']] = get_access('lx') + ' = tempDoublePtr;'
#CASES[ROPCODES['GETPC']] = get_access('lx') + ' = pc;'
CASES[ROPCODES['GETTR0']] = get_access('lx') + ' = tempRet0;'
CASES[ROPCODES['SETTR0']] = 'tempRet0 = ' + get_coerced_access('lx') + ';'

CASES[ROPCODES['INTCALL']] = '''
    inst = HEAP32[HEAP32[pc + 4 >> 2] >> 2] | 0; // FUNC inst: ['FUNC', locals, params, which emterp]
    lz = (inst >>> 16) & 255; // params
    ly = 0;
    assert(((EMTSTACKTOP + 8|0) <= (EMT_STACK_MAX|0))|0); // for return value
    if ((inst >>> 24) == 0) {
      while ((ly|0) < (lz|0)) {
        %s = %s;
        %s = %s;
        ly = ly + 1 | 0;
      }
      emterpret(HEAP32[pc + 4 >> 2] | 0);
    } else {
      while ((ly|0) < (lz|0)) {
        %s = %s;
        %s = %s;
        ly = ly + 1 | 0;
      }
      emterpret_z(HEAP32[pc + 4 >> 2] | 0);
    }
    %s = HEAP32[EMTSTACKTOP >> 2] | 0;
    %s = HEAP32[EMTSTACKTOP + 4 >> 2] | 0;
    pc = pc + (((4 + lz + 3) >> 2) << 2) | 0;
''' % (
  get_access('ly', base='EMTSTACKTOP'),     get_coerced_access('HEAPU8[pc + 8 + ly >> 0]'),
  get_access('ly', base='EMTSTACKTOP', offset=4), get_coerced_access('HEAPU8[pc + 8 + ly >> 0]', offset=4),
  get_access('ly', base=0),     get_coerced_access('HEAPU8[pc + 8 + ly >> 0]'),
  get_access('ly', base=0, offset=4), get_coerced_access('HEAPU8[pc + 8 + ly >> 0]', offset=4),
  get_access('lx'), get_access('lx', offset=4),
)

CASES[ROPCODES['SWITCH']] = '''
    lz = ''' + get_coerced_access('lz') + ''';
    lx = ((''' + get_coerced_access('lx') + ''') - (''' + get_coerced_access('ly') + ''')) >>> 0; // lx is now relative to the base
    if ((lx >>> 0) >= (lz >>> 0)) { // is the adjusted value too big?
      pc = (pc + (lz << 2)) | 0; // jump to right after the table, where the default is
      PROCEED_WITH_PC_BUMP; // also increment the pc normally, to skip the switch itself
    }
    pc = HEAP32[pc + 4 + (lx << 2) >> 2] | 0; // load from the jump table which is right after this instruction, and set pc
    PROCEED_WITHOUT_PC_BUMP;'''

def make_emterpreter(zero=False):
  # return is specialized per interpreter
  CASES[ROPCODES['RET']] = 'EMTSTACKTOP = sp; ' if not zero else ''
  CASES[ROPCODES['RET']] += 'HEAP32[EMTSTACKTOP >> 2] = ' + get_coerced_access('lx') + '; HEAP32[EMTSTACKTOP + 4 >> 2] = ' + get_coerced_access('lx', offset=4) + '; return;'

  # call is custom generated using information of actual call patterns, and which emterpreter this is
  def make_target_call(i):
    name = global_func_names[i]
    sig = global_func_sigs[i]

    function_pointer_call = name.startswith('FUNCTION_TABLE_')

    ret = name
    if function_pointer_call:
      ret += '[' + get_access('HEAPU8[pc+4>>0]') + ' & %d]' % (next_power_of_two(asm.tables[name].count(',')+1)-1)
    ret += '(' + ', '.join([get_coerced_access('HEAPU8[pc+%d>>0]' % (i+4+int(function_pointer_call)), s=sig[i+1]) for i in range(len(sig)-1)]) + ')'
    if sig[0] != 'v':
      ret = get_access('lx', sig[0]) + ' = ' + shared.JS.make_coercion(ret, sig[0])
    elif name in actual_return_types and actual_return_types[name] != 'v':
      ret = shared.JS.make_coercion(ret, actual_return_types[name]) # return value ignored, but need a coercion
    extra = len(sig) - 1 + int(function_pointer_call) # [opcode, lx, target, sig], take the usual 4. params are extra
    if extra > 0:
      ret += '; pc = pc + %d | 0' % (4*((extra+3)>>2))
    return '     ' + ret + '; PROCEED_WITH_PC_BUMP;'

  CASES[ROPCODES['EXTCALL']] = 'switch ((inst>>>16)|0) {\n' + \
    '\n'.join(['    case %d: {\n%s\n    }' % (i, make_target_call(i)) for i in range(global_func_id)]) + \
    '\n    default: assert(0);' + \
    '\n   }'

  if ROPCODES['GETGLBI'] not in CASES:
    def make_load(i):
      sig = 'i'
      name = rglobal_vars[i]
      return '     ' + get_access('lx', sig[0]) + ' = ' + name + '; PROCEED_WITH_PC_BUMP;'
    CASES[ROPCODES['GETGLBI']] = 'switch (ly|0) {\n' + \
      '\n'.join(['    case %d: {\n%s\n    }' % (i, make_load(i)) for i in range(global_var_id)]) + \
      '\n    default: assert(0);' + \
      '\n   }'
    def make_store(i):
      sig = 'i'
      name = rglobal_vars[i]
      return '     ' + name + ' = ' + get_coerced_access('lz', sig[0]) + '; PROCEED_WITH_PC_BUMP;'
    CASES[ROPCODES['SETGLBI']] = 'switch ((inst >> 8)&255) {\n' + \
      '\n'.join(['    case %d: {\n%s\n    }' % (i, make_store(i)) for i in range(global_var_id)]) + \
      '\n    default: assert(0);' + \
      '\n   }'

  def fix_case(case):
    # we increment pc at the top of the loop. to avoid a pc bump, we decrement it first; this is rare, most opcodes just continue; this avoids any code at the end of the loop
    return case.replace('PROCEED_WITH_PC_BUMP', 'continue').replace('PROCEED_WITHOUT_PC_BUMP', 'pc = pc - 4 | 0; continue').replace('continue; continue;', 'continue;')

  def process(code):
    code = code.replace(' assert(', ' //assert(')
    if zero: code = code.replace('sp + ', '')
    return code

  main_loop_prefix = r'''  //if (first) first = false; else print('last lx (' + lx + '): ' + [''' + get_coerced_access('lx') + ',' + get_coerced_access('lx', s='d') + ''']);
  pc = pc + 4 | 0;
  inst = HEAP32[pc>>2]|0;
  lx = (inst >> 8) & 255;
  ly = (inst >> 16) & 255;
  lz = inst >>> 24;
  //print([pc, inst&255, %s[inst&255], lx, ly, lz, HEAPU8[pc + 4],HEAPU8[pc + 5],HEAPU8[pc + 6],HEAPU8[pc + 7]].join(', '));
''' % (json.dumps(OPCODES))

  if not INNERTERPRETER_LAST_OPCODE:
    main_loop = main_loop_prefix + r'''
  switch (inst&255) {
%s
   default: assert(0);
  }
''' % ('\n'.join([fix_case('   case %d: %s break;' % (k, CASES[k])) for k in sorted(CASES.keys())]))
  else:
    # emit an inner interpreter (innerterpreter) loop, of trivial opcodes that hopefully the JS engine will implement with no spills
    assert OPCODES[-1] == 'FUNC' # we don't need to emit that one
    main_loop = r'''  innerterpreter: while (1) {
%s
   switch (inst&255) {
%s
%s
    default: break innerterpreter;
   }
  }
  switch (inst&255) {
%s
   default: assert(0);
  }
''' % (
  ' ' + '\n '.join(main_loop_prefix.split('\n')),
  '\n'.join([fix_case('    case %d: %s break;' % (ROPCODES[k], CASES[ROPCODES[k]])) for k in OPCODES[:-1][:ROPCODES[INNERTERPRETER_LAST_OPCODE]+1]]),
  '\n'.join([fix_case('    case %d:' % (ROPCODES[k])) for k in OPCODES[:-1][ROPCODES[INNERTERPRETER_LAST_OPCODE]+1:]]),
  '\n'.join([fix_case('   case %d: %s break;' % (ROPCODES[k], CASES[ROPCODES[k]])) for k in OPCODES[:-1][ROPCODES[INNERTERPRETER_LAST_OPCODE]+1:]])
)

  return process(r'''
function emterpret%s(pc) {
 pc = pc | 0;
 var %sinst = 0, lx = 0, ly = 0, lz = 0;
%s
 assert(((HEAPU8[pc>>0]>>>0) == %d)|0);
 lx = HEAPU8[pc + 1 >> 0] | 0; // num locals
%s
 ly = HEAPU8[pc + 2 >> 0] | 0; // first zeroinit (after params)
 lz = HEAPU8[pc + 4 >> 0] | 0; // offset of last zeroinit
 while ((ly | 0) < (lz | 0)) { // clear the zeroinits
  %s = +0;
  ly = ly + 1 | 0;
 }
 //print('enter func ' + [pc, HEAPU8[pc + 0],HEAPU8[pc + 1],HEAPU8[pc + 2],HEAPU8[pc + 3],HEAPU8[pc + 4],HEAPU8[pc + 5],HEAPU8[pc + 6],HEAPU8[pc + 7]].join(', '));
 //var first = true;
 pc = pc + 4 | 0;
 while (1) {
%s
 }
 assert(0);
}''' % (
  '' if not zero else '_z',
  'sp = 0, ' if not zero else '',
  ' sp = EMTSTACKTOP;' if not zero else '',
  ROPCODES['FUNC'],
  ''' EMTSTACKTOP = EMTSTACKTOP + (lx << 3) | 0;
 assert(((EMTSTACKTOP|0) <= (EMT_STACK_MAX|0))|0);''' if not zero else '',
  get_access('ly', s='d'),
  main_loop,
))

# main
if __name__ == '__main__':
  infile = sys.argv[1]
  outfile = sys.argv[2]
  force_memfile = sys.argv[3] if len(sys.argv) >= 4 else None
  extra_blacklist = json.loads(sys.argv[4]) if len(sys.argv) >= 5 else []

  BLACKLIST = set(list(BLACKLIST) + extra_blacklist)

  shared.logging.debug('saving original (non-emterpreted) code to ' + infile + '.orig.js')
  shutil.copyfile(infile, infile + '.orig.js')

  # final global functions

  asm = asm_module.AsmModule(infile)

  # sanity check on blacklist

  for func in extra_blacklist:
    assert func in asm.funcs, 'requested blacklist of %s but it does not exist' % func

  # decide which functions will be emterpreted, and find which are externally reachable (from outside other emterpreted code; those will need trampolines)

  emterpreted_funcs = set([func for func in asm.funcs if func not in BLACKLIST and not func.startswith('dynCall_')])

  tabled_funcs = asm.get_table_funcs()
  exported_funcs = [func.split(':')[0] for func in asm.exports]

  temp = infile + '.tmp.js'

  # find emterpreted functions reachable by non-emterpreted ones, we will force a trampoline for them later

  shared.Building.js_optimizer(infile, ['findReachable'], extra_info={ 'blacklist': list(emterpreted_funcs) }, output_filename=temp, just_concat=True)
  asm = asm_module.AsmModule(temp)
  lines = asm.funcs_js.split('\n')

  reachable_funcs = set([])
  for i in range(len(lines)):
    line = lines[i]
    if line.startswith('// REACHABLE '):
      curr = json.loads(line[len('// REACHABLE '):])
      reachable_funcs = set(list(reachable_funcs) + curr)

  external_emterpreted_funcs = filter(lambda func: func in tabled_funcs or func in exported_funcs or func in reachable_funcs, emterpreted_funcs)

  # process functions, generating bytecode
  shared.Building.js_optimizer(infile, ['emterpretify'], extra_info={ 'emterpretedFuncs': list(emterpreted_funcs), 'externalEmterpretedFuncs': list(external_emterpreted_funcs), 'opcodes': OPCODES, 'ropcodes': ROPCODES }, output_filename=temp, just_concat=True)

  # load the module and modify it
  asm = asm_module.AsmModule(temp)

  in_mem_file = infile + '.mem'
  in_mem_file_base = os.path.basename(in_mem_file)
  out_mem_file = outfile + '.mem'
  out_mem_file_base = os.path.basename(out_mem_file)

  assert in_mem_file_base in asm.pre_js, 'we assume a mem init file for now (looked for %s)' % in_mem_file
  if not force_memfile:
    asm.pre_js = asm.pre_js.replace(in_mem_file_base, out_mem_file_base)
    assert os.path.exists(in_mem_file), 'need to find mem file at %s' % in_mem_file
  else:
    out_mem_file = force_memfile
    out_mem_file_base = os.path.basename(out_mem_file)
  mem_init = map(ord, open(in_mem_file, 'rb').read())
  zero_space = asm.staticbump - len(mem_init)
  assert zero_space >= 0 # can be positive, if we add a bump of zeros

  assert ('GLOBAL_BASE: %d,' % GLOBAL_BASE) in asm.pre_js, 'we assume a specific global base, and that we can write to all memory below it'

  # calculate where code will start
  while len(mem_init) % 8 != 0:
    mem_init.append(0)
    asm.staticbump += 1
  code_start = len(mem_init) + GLOBAL_BASE

  # parse out bytecode and add to mem init file
  all_code = []
  funcs = {}
  lines = asm.funcs_js.split('\n')
  asm.funcs_js = None
  func = None

  # first pass, collect and process bytecode

  global_funcs = {} # 'name|sig' -> id
  global_func_names = {} # id -> name
  global_func_sigs = {} # id -> sig, one name can have multiple sigs
  global_func_id = 0

  global_vars = {}
  rglobal_vars = {}
  global_var_id = 0

  call_sigs = {} # signatures appearing for each call target
  def process_code(func, code, absolute_targets):
    global global_func_id
    global global_var_id
    absolute_start = code_start + len(all_code) # true absolute starting point of this function
    #print 'processing code', func, absolute_start
    for i in range(len(code)/4):
      j = i*4
      if code[j] == 'EXTCALL':
        # fix CALL instructions' targets and signatures
        target = code[j+2]
        sig = code[j+3]
        if target not in call_sigs: call_sigs[target] = []
        sigs = call_sigs[target]
        if sig not in sigs: sigs.append(sig)
        fullname = target + '|' + sig
        if fullname not in global_funcs:
          global_funcs[fullname] = global_func_id
          global_func_names[global_func_id] = target
          global_func_sigs[global_func_id] = sig
          global_func_id += 1
        code[j+2] = global_funcs[fullname] & 255
        code[j+3] = global_funcs[fullname] >> 8
        if sig[0] == 'v':
          if code[j+1] == -1: # dummy value for assignment XXX we should not have assignments on void calls
            code[j+1] = 0 # clear it
        else:
          assert code[j+1] >= 0 # there should be a real target here
      elif code[j] in ['GETGLBI', 'GETGLBD']:
        # fix global-accessing instructions' targets
        target = code[j+2]
        imp = asm.imports[target]
        assert '|0' in imp or '| 0' in imp or imp == '0'
        if target not in global_vars:
          global_vars[target] = global_var_id
          rglobal_vars[global_var_id] = target
          global_var_id += 1
        code[j+2] = global_vars[target]
      elif code[j] in ['SETGLBI']:
        # fix global-accessing instructions' targets
        target = code[j+1]
        if target not in global_vars:
          global_vars[target] = global_var_id
          rglobal_vars[global_var_id] = target
          global_var_id += 1
        code[j+1] = global_vars[target]
      elif code[j] == 'absolute-value':
        # put the 32-bit absolute value of an abolute target here
        absolute_value = absolute_start + absolute_targets[unicode(code[j+1])]
        #print '  fixing absolute value', code[j+1], absolute_targets[unicode(code[j+1])], absolute_value
        assert absolute_value < (1 << 31)
        assert absolute_value % 4 == 0
        value = bytify(absolute_value)
        for k in range(4):
          code[j + k] = value[k]

  actual_return_types = {}

  for i in range(len(lines)):
    line = lines[i]
    if line.startswith('function ') and '}' not in line:
      assert not func
    elif line.startswith('// EMTERPRET_INFO '):
      try:
        func, curr, absolute_targets = json.loads(line[len('// EMTERPRET_INFO '):])
      except Exception, e:
        print >> sys.stderr, 'failed to parse code from', line
        raise e
      assert len(curr) % 4 == 0, curr
      funcs[func] = len(all_code) # no operation here should change the length
      if LOG_CODE: print >> sys.stderr, 'raw bytecode for %s:' % func, curr, 'insts:', len(curr)/4
      process_code(func, curr, absolute_targets)
      #print >> sys.stderr, 'processed bytecode for %s:' % func, curr
      all_code += curr
      func = None
      lines[i] = ''
    elif line.startswith('// return type: ['):
      name, ret = line.split('[')[1].split(']')[0].split(',')
      if ret == 'undefined':
        actual_return_types[name] = 'v'
      elif ret == '0':
        actual_return_types[name] = 'i'
      elif ret == '1':
        actual_return_types[name] = 'd'
      elif ret == '2':
        actual_return_types[name] = 'f'
      lines[i] = ''

  assert global_func_id < 65536, [global_funcs, global_func_id]
  assert global_var_id < 256, [global_vars, global_var_id]

  def post_process_code(code):
    for i in range(len(code)/4):
      j = i*4
      if code[j] == 'absolute-funcaddr':
        # put the 32-bit absolute value of an abolute function here
        absolute_value = code_start + funcs[code[j+1]]
        #print '  fixing absolute value', code[j+1], absolute_targets[unicode(code[j+1])], absolute_value
        assert absolute_value < (1 << 31)
        assert absolute_value % 4 == 0
        value = bytify(absolute_value)
        for k in range(4):
          code[j + k] = value[k]

    # finalize instruction string names to opcodes
    for i in range(len(code)/4):
      j = i*4
      if type(code[j]) in (str, unicode):
        code[j] = ROPCODES[code[j]]

    # sanity checks
    for i in range(len(code)):
      v = code[i]
      assert type(v) == int and v >= 0 and v < 256, [i, v, 'in', code]

  post_process_code(all_code)

  # create new mem init
  mem_init = mem_init + all_code
  asm.staticbump += len(all_code)

  while len(mem_init) % 8 != 0:
    mem_init.append(0)
    asm.staticbump += 1
  stack_start = len(mem_init)
  asm.staticbump += EMT_STACK_MAX

  open(out_mem_file, 'wb').write(''.join(map(chr, mem_init)))

  # second pass, finalize trampolines
  for i in range(len(lines)):
    line = lines[i]
    if line.startswith('function ') and '}' not in line:
      assert not func
      func = line.split(' ')[1].split('(')[0]
    elif line.startswith('}'):
      assert func
      func = None
    elif func and func in funcs:
      call = '(EMTERPRETER_' + func + ')'
      if call in line:
        lines[i] = lines[i].replace(call, '(%s)' % (funcs[func] + code_start))

  # finalize funcs JS (first line has the marker, add emterpreters right after that)
  asm.funcs_js = '\n'.join([lines[0], make_emterpreter(), make_emterpreter(zero=True), '\n'.join(filter(lambda line: len(line) > 0, lines[1:]))]) + '\n'
  lines = None

  # set up emterpreter stack top
  asm.set_pre_js(js='var EMTSTACKTOP = STATIC_BASE + %s, EMT_STACK_MAX = EMTSTACKTOP + %d;' % (stack_start, EMT_STACK_MAX))

  # send EMT vars into asm
  asm.pre_js += 'Module.asmLibraryArg.EMTSTACKTOP = EMTSTACKTOP; Module.asmLibraryArg.EMT_STACK_MAX = EMT_STACK_MAX;\n'
  asm.imports_js += 'var EMTSTACKTOP = env.EMTSTACKTOP|0;\nvar EMT_STACK_MAX = env.EMT_STACK_MAX|0;\n'

  asm.write(outfile)

