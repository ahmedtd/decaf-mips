#!/bin/sh

cd ../solution
./dcc < ../samples/$1 > ../samples/solution_tmpfile 2>&1

cd ../
./dcc < samples/$1 > samples/test_tmpfile 2>&1

cd samples
diff -w test_tmpfile solution_tmpfile > diff_tmpfile 2>&1

difflength=`wc -ac < diff_tmpfile`

if [ "$difflength" -ne "0" ]
then
    echo "---------------------------------------------------------------------"
    echo ">>>>>>>>>>>>>>>>>>>>>>>>"
    echo "Solution output:"
    echo ">>>>>>>>>>>>>>>>>>>>>>>>"
    cat solution_tmpfile
    echo "<<<<<<<<<<<<<<<<<<<<<<<<"
    echo "Test output:"
    echo "<<<<<<<<<<<<<<<<<<<<<<<<"
    cat test_tmpfile
    echo "++++++++++++++++++++++++"
    echo "Diff Output:"
    echo "++++++++++++++++++++++++"
    cat diff_tmpfile
    echo "---------------------------------------------------------------------"

    retcode=1
else
    retcode=0
fi

rm solution_tmpfile test_tmpfile diff_tmpfile
exit $retcode
