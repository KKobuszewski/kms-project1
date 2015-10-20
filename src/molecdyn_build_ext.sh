PYTHON=python2.7

sudo rm /usr/local/lib/python2.7/dist-packages/Molec*
rm -r build

$PYTHON --version
$PYTHON setup.py build
sudo $PYTHON setup.py install
# $PYTHON setup.py build_ext -i
$PYTHON test.py