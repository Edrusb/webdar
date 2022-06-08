#!/bin/bash

if [ -z "$1" ] ; then
    echo "$0 <prototype file>"
    exit 1
fi

normalize_name()
{
    if [ -z "$1" ] ; then
	echo "$0 <word>"
	exit 1
    fi

    echo "$1" | sed -e 's/-/_/g'
}

for selname in link active visited hover checked enabled disabled focus valid invalid in-range out-of-range read-only read-write ; do
    norm_selname=`normalize_name $selname`
    sed -r -e "s/SELNAME/$selname/g" -e "s/NORM_NAME/$norm_selname/g" < "$1"
done

