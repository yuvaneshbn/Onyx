param(
    [string]$Destination = (Join-Path $PSScriptRoot "..\thirdparty")
)

$ErrorActionPreference = "Stop"

function Expand-ArchiveToFolder {
    param(
        [Parameter(Mandatory = $true)][string]$ArchivePath,
        [Parameter(Mandatory = $true)][string]$TargetFolder
    )

    $tempFolder = Join-Path ([System.IO.Path]::GetTempPath()) ([System.Guid]::NewGuid().ToString())
    New-Item -ItemType Directory -Force -Path $tempFolder | Out-Null
    Expand-Archive -LiteralPath $ArchivePath -DestinationPath $tempFolder -Force

    $topLevel = Get-ChildItem -LiteralPath $tempFolder | Select-Object -First 1
    if (-not $topLevel) {
        throw "Archive $ArchivePath did not contain any files."
    }

    if (Test-Path -LiteralPath $TargetFolder) {
        Write-Host "Skipping existing $TargetFolder"
    } else {
        Move-Item -LiteralPath $topLevel.FullName -Destination $TargetFolder
        Write-Host "Extracted to $TargetFolder"
    }

    Remove-Item -LiteralPath $tempFolder -Recurse -Force
}

function Expand-TarGzToFolder {
    param(
        [Parameter(Mandatory = $true)][string]$ArchivePath,
        [Parameter(Mandatory = $true)][string]$TargetFolder
    )

    $tempFolder = Join-Path ([System.IO.Path]::GetTempPath()) ([System.Guid]::NewGuid().ToString())
    New-Item -ItemType Directory -Force -Path $tempFolder | Out-Null
    tar -xf $ArchivePath -C $tempFolder

    $topLevel = Get-ChildItem -LiteralPath $tempFolder | Select-Object -First 1
    if (-not $topLevel) {
        throw "Archive $ArchivePath did not contain any files."
    }

    if (Test-Path -LiteralPath $TargetFolder) {
        Write-Host "Skipping existing $TargetFolder"
    } else {
        Move-Item -LiteralPath $topLevel.FullName -Destination $TargetFolder
        Write-Host "Extracted to $TargetFolder"
    }

    Remove-Item -LiteralPath $tempFolder -Recurse -Force
}

$destinationPath = (Resolve-Path $Destination).Path
$downloadPath = Join-Path $destinationPath "_downloads"
New-Item -ItemType Directory -Force -Path $downloadPath | Out-Null

$dependencies = @(
    @{
        Name = "jsoncpp-1.9.6"
        Url = "https://github.com/open-source-parsers/jsoncpp/archive/refs/tags/1.9.6.zip"
        ArchiveType = "zip"
    },
    @{
        Name = "hidapi-hidapi-0.14.0"
        Url = "https://github.com/libusb/hidapi/archive/refs/tags/hidapi-0.14.0.zip"
        ArchiveType = "zip"
    },
    @{
        Name = "libvpx-1.14.1"
        Url = "https://github.com/webmproject/libvpx/archive/refs/tags/v1.14.1.zip"
        ArchiveType = "zip"
    },
    @{
        Name = "opencore-amr-0.1.6"
        Url = "https://distro.ibiblio.org/ubuntu/ubuntu/pool/universe/o/opencore-amr/opencore-amr_0.1.6.orig.tar.gz"
        ArchiveType = "targz"
    },
    @{
        Name = "vo-amrwbenc"
        Url = "https://github.com/mstorsjo/vo-amrwbenc/archive/refs/heads/master.zip"
        ArchiveType = "zip"
    }
)

foreach ($dependency in $dependencies) {
    $archiveName = if ($dependency.ArchiveType -eq "targz") {
        "{0}.tar.gz" -f $dependency.Name
    } else {
        "{0}.zip" -f $dependency.Name
    }
    $archiveFile = Join-Path $downloadPath $archiveName
    $targetFolder = Join-Path $destinationPath $dependency.Name

    if (Test-Path -LiteralPath $targetFolder) {
        Write-Host "Already present: $targetFolder"
        continue
    }

    Write-Host "Downloading $($dependency.Url)"
    Invoke-WebRequest -Uri $dependency.Url -OutFile $archiveFile

    if ($dependency.ArchiveType -eq "targz") {
        Expand-TarGzToFolder -ArchivePath $archiveFile -TargetFolder $targetFolder
    } else {
        Expand-ArchiveToFolder -ArchivePath $archiveFile -TargetFolder $targetFolder
    }
}

Write-Host ""
Write-Host "Third-party bootstrap finished."
Write-Host "Manual follow-up is still required for Intel IPP and for any pjproject .lib files you want to link on Windows."
