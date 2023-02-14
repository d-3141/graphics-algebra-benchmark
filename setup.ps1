# Check if Scoop (a Windows package manager) is available.
if (Get-Command "scoop" -ErrorAction SilentlyContinue) { 
   Write-Output "Scoop is available."
}
else {
    # Install Scoop.
    Set-ExecutionPolicy RemoteSigned -Scope CurrentUser
    Invoke-RestMethod get.scoop.sh | Invoke-Expression
}

# Install Scoop packages.
scoop bucket add main
scoop install git
scoop install cmake
scoop install vcpkg
scoop update

vcpkg install
vcpkg integrate install
