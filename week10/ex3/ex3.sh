echo "hello world" > _ex3.txt
echo "remove execute permission for everybody"
chmod a-x _ex3.txt # remove execute permission for everybody
ls -l _ex3.txt # see what happened
echo "add all permissions to owner"
chmod u+rwx _ex3.txt # add all permissions to owner
ls -l _ex3.txt # see what happened
echo "add all permissions to other users"
chmod o+rwx _ex3.txt #add all permission to other users
ls -l _ex3.txt # see what happened
echo "set group permissions equal to user ones"
chmod g=u _ex3.txt # set group permissions equal to user 
ls -l _ex3.txt # see what happened

