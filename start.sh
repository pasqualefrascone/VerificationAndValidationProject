#!/bin/bash

#docker compose up -d server
#docker compose up -d client
#sleep 10
(cat number | docker exec -i client1 ./client CLIENT1) &
cat number | docker exec -i client1 ./client CLIENT2 &
cat number | docker exec -i client1 ./client CLIENT3 &
cat number | docker exec -i client1 ./client CLIENT4 &
cat number | docker exec -i client1 ./client CLIENT5 &
cat number | docker exec -i client1 ./client clientACaso &
cat number | docker exec -i client1 ./client CLIENT6 &
cat number | docker exec -i client1 ./client CLIENT7 &
cat number | docker exec -i client1 ./client CLIENT8 &
cat number | docker exec -i client1 ./client RANDOMCLIENT &


#dig -x `ifconfig eth0 | grep 'inet' | awk '{print $2}'` +short | cut -d'.' -f1