
var MAGIC = 0;
Math.random = function() {
  MAGIC = Math.pow(MAGIC + 1.8912, 3) % 1;
  return MAGIC;
};
var TIME = 10000;
Date.now = function() {
  return TIME++;
};
if (typeof performance === 'object') performance.now = Date.now;

function hashMemory(id) {
  var ret = 0;
  var len = Math.max(DYNAMICTOP, STATICTOP);
  for (var i = 0; i < len; i++) {
    ret = (ret*17 + HEAPU8[i])|0;
  }
  printErr(id + ':' + ret);
}

