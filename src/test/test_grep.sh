#!/bin/bash

DIRECTORY="."
MYGREP="../grep/s21_grep"
PATTERNS="^h 65"
FLAGS="-e -i -v -c -l -n -h -o"
DIFFRNC=""
FAILFLAGS=""
SUCCESS=0
FAIL=0
TOTAL=0

if [ ! -d "$DIRECTORY" ]
then
  echo "Folder does not exists bro: $DIRECTORY"
  exit 1
fi

files=("$DIRECTORY"/*.txt)
count=${#files[@]}

if [ -e "s21_grep_tmp.txt" ]
then
    rm s21_grep_tmp.txt sys_grep_tmp.txt
fi

# 1 file
for flag in $FLAGS
do
    for patrn in $PATTERNS
    do
        for ((i=0; i<${#files[@]}; i+=1))
        do            
                $MYGREP $flag $patrn ${files[i]} > s21_grep_tmp.txt
                grep $flag $patrn ${files[i]}> sys_grep_tmp.txt
                DIFFRNC="$(diff s21_grep_tmp.txt sys_grep_tmp.txt)"
                ((TOTAL++))
                if [ "$DIFFRNC" == "" ]
                    then
                        ((SUCCESS++))
                    else
                        ((FAIL++))
                        FAILFLAGS="${FAILFLAGS} ${flag} ${patrn} ${files[i]} \n"
                fi
                rm s21_grep_tmp.txt sys_grep_tmp.txt
        done
    done
done

# 2 files
for flag in -e -i -v -c -l -n
do
    for patrn in $PATTERNS
    do
        for ((i=0; i<${#files[@]}; i+=1))
        do
            for ((j=count-1; j>=0; j-=1))
            do            
                $MYGREP $flag $patrn ${files[i]} ${files[j]} > s21_grep_tmp.txt
                grep $flag $patrn ${files[i]} ${files[j]} > sys_grep_tmp.txt
                DIFFRNC="$(diff s21_grep_tmp.txt sys_grep_tmp.txt)"
                ((TOTAL++))
                if [ "$DIFFRNC" == "" ]
                    then
                        ((SUCCESS++))
                    else
                        ((FAIL++))
                        FAILFLAGS="${FAILFLAGS} ${flag} ${files[i]} ${files[j]} \n"
                fi
                rm s21_grep_tmp.txt sys_grep_tmp.txt
            done
        done
    done
done

echo "SUCCESS: $SUCCESS"
echo "FAIL: $FAIL"
echo "TOTAL: $TOTAL"
if [ ! "$FAILFLAGS" == "" ]
then
    echo -e "Failed at flags: \n $FAILFLAGS"
fi