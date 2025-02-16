#!/bin/bash

DIRECTORY="."
MYCAT="../cat/s21_cat"
FLAGS="-b -e -n -s -t -v -E -T"
DIFFRNC=""
FAILFLAGS=""
SUCCESS=0
FAIL=0
TOTAL=0

files=(./*.txt)
count=${#files[@]}

if [ -e "s21_cat_tmp.txt" ]
then
    rm s21_cat_tmp.txt sys_cat_tmp.txt
fi

# 1 file
for flag in $FLAGS
do
    for ((i=0; i<${#files[@]}; i+=1))
    do                    
        $MYCAT $flag ${files[i]} > s21_cat_tmp.txt
        cat $flag ${files[i]} > sys_cat_tmp.txt
        DIFFRNC="$(diff s21_cat_tmp.txt sys_cat_tmp.txt)"
        ((TOTAL++))
        if [ "$DIFFRNC" == "" ]
            then
                ((SUCCESS++))
            else
                ((FAIL++))
                FAILFLAGS="${FAILFLAGS} ${flag} ${files[i]} \n"
        fi
        rm s21_cat_tmp.txt sys_cat_tmp.txt        
    done
done

# 2 files
for flag in $FLAGS
do
    for ((i=0; i<${#files[@]}; i+=1))
    do
        for ((j=count-1; j>=0; j-=1))
        do            
            $MYCAT $flag ${files[i]} ${files[j]} > s21_cat_tmp.txt
            cat $flag ${files[i]} ${files[j]} > sys_cat_tmp.txt
            DIFFRNC="$(diff s21_cat_tmp.txt sys_cat_tmp.txt)"
            ((TOTAL++))
            if [ "$DIFFRNC" == "" ]
                then
                    ((SUCCESS++))
                else
                    ((FAIL++))
                    FAILFLAGS="${FAILFLAGS} ${flag} ${files[i]} ${files[j]} \n"
            fi
            rm s21_cat_tmp.txt sys_cat_tmp.txt
        done
    done
done

echo "SUCCESS: $SUCCESS"
echo "FAIL: $FAIL"
echo "TOTAL: $TOTAL"
if [ ! "$FAILFLAGS" == "" ]
then
    echo -e "Failed at flags: \n$FAILFLAGS"
fi