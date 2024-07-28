P0=$1
P1=$2
DATA=$3

echo $DATA

echo $P0
time cat $DATA | $P0 | wc

echo $P1
time cat $DATA | $P1 | wc

echo diff:
diff <(cat $DATA | $P0 | sort) <(cat $DATA | $P1 | sort)
echo END
