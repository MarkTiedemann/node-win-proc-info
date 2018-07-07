const bindings = require('bindings')
const addon = bindings('proc_info.node')
module.exports = addon.proc_info
