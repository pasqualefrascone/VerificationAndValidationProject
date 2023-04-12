#!/bin/bash

#clientName=$1
DB_USER="postgres"
DB_PWD="password"
DB_NAME="sistema"
HOST_NAME="localhost"

test "$1" = "clean" && 
    (docker exec -t pdb psql -P pager=off  -U postgres "postgresql://$DB_USER:$DB_PWD@$HOST_NAME/$DB_NAME" -c "delete from agent.logs;");
    
    #set POSTGRES_PASSWORD=password&& psql -U postgres -d sistema -w -c 'select * from agent.logs where who=${clientname}'
docker exec -t pdb psql -P pager=off  -U postgres "postgresql://$DB_USER:$DB_PWD@$HOST_NAME/$DB_NAME" -c "select * from agent.logs order by whenn ASC"
#docker exec -t pdb psql -P pager=off  -U postgres "postgresql://$DB_USER:$DB_PWD@$HOST_NAME/$DB_NAME" -c "select * from agent.logs where who='client' order by whenn ASC"