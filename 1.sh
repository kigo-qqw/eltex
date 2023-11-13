N=10

for i in $(seq 1 1 $N);
do
  sleep 60 &
done;

wait

echo $N > N.txt

sleep 3600
