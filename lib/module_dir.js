// We build different versions of the binary module in different subdirectories, similar to
// node-pre-gyp; module_dir() returns the appropriate subdirectory name.
function module_dir() {
  const runtime = get_runtime();
  const abi = (runtime === 'electron') ? get_electron_abi() : process.versions.modules;
  return `${runtime}-v${abi}-${process.platform}-${process.arch}`;
}

function get_electron_abi() {
  // We use the version number instead of the actual ABI number because we might run with only the
  // version passed-in via the npm environment, without having electron available.
  const version = process.env.npm_config_target || process.versions.electron;
  const [major, minor] = version.split('.');
  return `${major || 0}.${minor || 0}`;
}

function get_runtime() {
  return process.env.npm_config_runtime || (process.versions.electron ? 'electron' : 'node');
}

module.exports = module_dir;

if (require.main === module) {
  console.log(module_dir());
}
