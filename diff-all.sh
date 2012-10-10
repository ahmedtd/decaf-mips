#!/bin/sh

# Arg $1: Folder containing our dcc and dpp
# Arg $2: Folder containing solution dcc and dpp
# Arg $3: Directory containing some decaf programs to test

DIF_TMPFILE=$(mktemp)

for curfile in $3/*.decaf
do
    ./diff-test.sh "$1" "$2" "$curfile" > "$DIF_TMPFILE" 2>&1
    OUTLEN=$(wc -c < "$DIF_TMPFILE")
    if [ "$OUTLEN" -ne "0" ]
    then
	echo "$curfile FAILED:"
	cat "$DIF_TMPFILE"

	rm $DIF_TMPFILE
	exit 1
    else
	echo "$curfile PASSED"
    fi

done

rm "$DIF_TMPFILE"
exit 0
