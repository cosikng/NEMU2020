#!/bin/bash
extens=(".c" ".h")
filesCount=0
linesCount=0
function funCount() {
    # echo $1 $2 $3 $4 $5 $6 $7 $8
    # echo $@
    for file in $(ls $2); do
        if [ -d $2"/"$file ]; then
            funCount $1 $2"/"$file
        else
            fileName=$2"/"$file

            EXTENSION="."${fileName##*.}
            #    echo "fileName = $fileName  Extension = $EXTENSION"

            if [[ "${extens[@]/$EXTENSION/}" != "${extens[@]}" ]]; then
                declare -i fileLines
                if [ $1 = 'y' ];then
                    fileLines=`sed -n '$=' $fileName`
                else
                fileLines=$(cat $fileName | tr -s '\n' | sed -n '$=')
                fi
#                echo $fileName" : "$fileLines
                let linesCount=$linesCount+$fileLines
                let filesCount=$filesCount+1
            fi
        fi
    done
}
if [ $# -gt 1 ]; then
    for m_dir in $@; do
        funCount $1 $m_dir
    done
else
    funCount $1 "."
fi
echo "filesCount = $filesCount"
echo "linesCount = $linesCount"

