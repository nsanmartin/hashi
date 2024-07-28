P0=$1 # program to compare ex. stdin-count
P1=$2 # second program to compare ex. stdin-count-cppmap
DATA=$3 # file with data to fill P0 and P1's stdin

echo $DATA # print data source
echo $P0
time cat $DATA | $P0 | wc

echo $P1
time cat $DATA | $P1 | wc

echo diff:
diff <(cat $DATA | $P0 | sort) <(cat $DATA | $P1 | sort)
echo END
