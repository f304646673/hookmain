if [ ! -d "./output" ];then
	echo ""
else
	rm -rf ./output
fi
mkdir ./output
cd inject
sh build.sh
cp inject.so ../output
cd ..
cd child 
sh build.sh
cp child ../output
cd ..
cd parent
sh build.sh
cp parent ../output
cd ..
