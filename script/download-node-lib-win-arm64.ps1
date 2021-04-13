# This script can be removed as soon as official Windows arm64 builds are published:
# https://github.com/nodejs/build/issues/2450#issuecomment-705853342

$nodeVersion = $args[0]
$fallbackVersion = $args[1]

If ($null -eq $nodeVersion -Or $null -eq $fallbackVersion) {
  Write-Error "No NodeJS version given as argument to this file. Run it like download-nodejs-win-arm64.ps1 NODE_VERSION NODE_FALLBACK_VERSION"
  exit 1
}

$url = "https://unofficial-builds.nodejs.org/download/release/v$nodeVersion/win-arm64/node.lib"
$fallbackUrl = "https://unofficial-builds.nodejs.org/download/release/v$fallbackVersion/win-arm64/node.lib"

# Always write to the $nodeVersion cache folder, even if we're using the fallbackVersion
$cacheFolder = "$env:TEMP\prebuild\napi\$nodeVersion\arm64"

If (!(Test-Path $cacheFolder)) {
  New-Item -ItemType Directory -Force -Path $cacheFolder
}

$output = "$cacheFolder\node.lib"
$start_time = Get-Date

Try {
  Invoke-WebRequest -Uri $url -OutFile $output
  $downloadedNodeVersion = $nodeVersion
} Catch {
  If ($_.Exception.Response -And $_.Exception.Response.StatusCode -eq "NotFound") {
    Write-Output "No arm64 node.lib found for Node Windows $nodeVersion, trying fallback version $fallbackVersion..."
    Invoke-WebRequest -Uri $fallbackUrl -OutFile $output
    $downloadedNodeVersion = $fallbackVersion
  }
}

Write-Output "Downloaded arm64 NodeJS lib v$downloadedNodeVersion to $output in $((Get-Date).Subtract($start_time).Seconds) second(s)"
