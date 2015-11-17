PYTHON=python2.7

sudo rm /usr/local/lib/python2.7/dist-packages/Molec*
sudo rm /usr/local/lib/python2.7/dist-packages/Molec*
rm -r build

make clean
make 
#-e DVERBOSE
if [ ! \( -L "/usr/local/lib/python2.7/dist-packages/libmolecdyn.so" \) ]
then
    sudo ln -s ../lib/libmolecdyn.so /usr/local/lib/python2.7/dist-packages/libmolecdyn.so
fi

echo "Installing extension for:" $PYTHON --version
$PYTHON setup.py build
sudo $PYTHON setup.py install
# $PYTHON setup.py build_ext -i
$PYTHON test.py