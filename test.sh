#!/bin/bash
function measureTime ( ) {
    if [[ -z $1 ]]; then
        echo "Shit"
        exit
    fi
    if [[ "$1" == "example" || "$1" == "SKIP_PRINTOUT" ]]; then
        begin=`date +%s%N` &&
        bin/example > ./output-example
        end=`date +%s%N` &&
        if [[ "$1" == "SKIP_PRINTOUT" ]]; then
            echo "Execution time for eFScru was `expr $end - $begin` nano seconds."
        else
            echo "Execution time for eFScru was `expr $end - $begin` nano seconds. (skipped printing of directories)"
        fi
    elif [[ "$1" == "tree" ]]; then
        begin=`date +%s%N` &&
        tree -d "/usr/share/doc" > ./output-tree
        end=`date +%s%N` &&
        echo "Execution time for   tree was `expr $end - $begin` nano seconds."
    elif [[ "$1" == "example2" ]]; then
        begin=`date +%s%N` &&
        bin/example > ./output-example "don't" "preallocate" "for vector."
        end=`date +%s%N` &&
        echo "Execution time for eFScru was `expr $end - $begin` nano seconds (without preallocating memory for vector)."
    fi
}

function formatDuration {
    nanoseconds=`expr $end - $begin`
    milliseconds=$(awk -v nanosec=$nanoseconds "BEGIN {tmp =nanosec/1000000; if(tmp>10.0) printf \"ms % 5d\", tmp}") # 0.000001 for significant figures
    seconds=$(awk -v nanosec=$nanoseconds "BEGIN { tmp =nanosec/1000000000; if(tmp>1.0) printf \"s% 5d\", tmp}")

    durationString=$(awk -v nanosec=$nanoseconds -v ar1="$1"  -v ar2="$2" "BEGIN {
        tmp_ms=nanosec/1000000;
         tmp_s=nanosec/1000000000;
         tmp_min=nanosec/60000000000;

        if(tmp_ms > 10.0) {
            if (tmp_min > 0.25 ) printf \"%5s%8s\", tmp_min, \"min\"; #printf \"s% 5d\", tmp_min
            else
            if(tmp_s > 1.0) printf \"%5s%8s\", tmp_s, \"sec\"; #printf \"s% 5d\", tmp_s
            else printf \"%5s%8s\", tmp_ms, \"ms\"; #printf \"ms% 5d\", tmp_ms
        } else printf \"%5s%8s\", nanosec, \"ns\"; #printf \"ns% 5d\", nanosec
        printf \"     %s%40.25s\n\", ar1, ar2
    }")
    printf "%s\n" "$durationString"
    #if [[ ! -z "${seconds}" ]]; then
    #    printf "%s%5.4s s on: " $seconds;
    #elif [ ! -z "${milliseconds}" ]; then
    #    printf "%s%5.4s ms on: " $milliseconds;
    #else 
    #    printf "%s%5.4s ns on: " $nanoseconds;
    #fi
    #printf "%s%8s: $nanoseconds ns%s" "$1" " ($2)"
    #printf "\n"
}

function _timeTest {
    begin=`date +%s%N`
    sleep $1
    end=`date +%s%N`
    formatDuration "timer test" "$2"
}

function timeTest {
    _timeTest 0 "nanosec."
    _timeTest 2 "sec."
    _timeTest 15 "min."
}


begin=`date +%s%N`
bin/example > ./output-regular
end=`date +%s%N`
formatDuration "eFScru" "regular"

begin=`date +%s%N`
bin/example "SKIP_PRINTOUT" > ./output-skipPrint
end=`date +%s%N`
formatDuration "eFScru" "no printout"

begin=`date +%s%N`
bin/example "SKIP_PREALLOCATION"  > ./output-skipAlloc
end=`date +%s%N`
formatDuration "eFScru" "no pre allocation"

begin=`date +%s%N`
bin/example "SKIP_PREALLOCATION" "SKIP_PRINTOUT"  > ./output-skipAllocAndPrint
end=`date +%s%N`
formatDuration "eFScru" "no printout, no pre allocation"

begin=`date +%s%N`
tree -dfi "/usr/share/doc" > ./output-tree
end=`date +%s%N`
formatDuration "tree" "-dfi"
