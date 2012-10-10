#!/bin/sh

# Arg $1 : Folder containing our dcc and dpp
# Arg $2 : Folder containing solution dcc and dpp
# Arg $3 : Test file

SOL_TMPFILE=$(mktemp)
OUR_TMPFILE=$(mktemp)
DIF_TMPFILE=$(mktemp)

INPUT=$(readlink -f "$3")

pushd $2 > /dev/null
./dcc < "$INPUT" > "$SOL_TMPFILE" 2>&1
popd > /dev/null

pushd $1 > /dev/null 
./dcc < "$INPUT" > "$OUR_TMPFILE" 2>&1
popd > /dev/null

#cd samples
diff -w "$SOL_TMPFILE" "$OUR_TMPFILE" > "$DIF_TMPFILE" 2>&1

difflength=$(wc -c < "$DIF_TMPFILE")

if [ "$difflength" -ne "0" ]
then
    echo "---------------------------------------------------------------------"
    echo ">>>>>>>>>>>>>>>>>>>>>>>>"
    echo "Solution output:"
    echo ">>>>>>>>>>>>>>>>>>>>>>>>"
    cat "$SOL_TMPFILE"
    echo "<<<<<<<<<<<<<<<<<<<<<<<<"
    echo "Test output:"
    echo "<<<<<<<<<<<<<<<<<<<<<<<<"
    cat "$OUR_TMPFILE"
    echo "++++++++++++++++++++++++"
    echo "Diff Output:"
    echo "++++++++++++++++++++++++"
    cat "$DIF_TMPFILE"
    echo "---------------------------------------------------------------------"

    retcode=1
else
    retcode=0
fi

rm $SOL_TMPFILE $OUR_TMPFILE $DIF_TMPFILE
exit $retcode
