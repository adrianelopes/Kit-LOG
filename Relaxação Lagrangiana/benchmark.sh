make

k=1

for instance in instances/*; do
	echo $instance >> ./DFS.txt
	
	echo "DFS"
	echo "Processando $instance"
	echo "Instance $k of 15"
	
	./tsp ${instance} 1 >> ./DFS.txt
	
	k=$(($k+1))
done

echo "-" >> ./DFS.txt

