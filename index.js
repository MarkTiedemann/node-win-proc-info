const bindings = require('bindings')

const addon = bindings('proc_info.node')

module.exports = function procInfo(pid) {
  if (Number.isSafeInteger(pid)) {
    return addon.get_proc_info(pid)
  } else {
    throw new RangeError('invalid argument: pid must be a safe integer')
  }
}
