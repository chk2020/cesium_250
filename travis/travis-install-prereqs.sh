if [[ $TRAVIS_OS_NAME == "windows" ]]
then
  choco install 7zip.portable
  choco install python --version 3.9.2
  choco install wget
  choco install ninja
  python -m pip install --upgrade pip
  pip3 install --upgrade pip
  pip3 install awscli
  pip3 install httpie
  pip3 install conan
elif [[ $TRAVIS_OS_NAME == "osx" ]]
then
  python -m pip install --upgrade pip
  pip3 install --upgrade pip
  pip3 install awscli
  pip3 install httpie
  pip3 install conan
elif [[ $TRAVIS_OS_NAME == "linux" ]]
then
  choco install 7zip.portable
  choco install python --version 3.9.2
  choco install wget
  choco install ninja
  python -m pip install --upgrade pip
  pip3 install --upgrade pip
  pip3 install awscli
  pip3 install httpie
  pip3 install conan
fi

# On some platforms, it's python3, on others it's just python
export CESIUM_PYTHON=python3
if ! command -v python3
then
  export CESIUM_PYTHON=python
fi

# Install our custom version of conan-ue4cli
pushd ..
git clone -b cesium https://github.com/kring/conan-ue4cli.git
cd conan-ue4cli
$CESIUM_PYTHON ./setup.py install
popd