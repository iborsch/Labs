from random import seed, randint
from copy import copy

sec = open("secret.txt", "w")
log = open("log.txt", "w")

def calculate(my, their):
    bulls = 0
    cows = 0
    for i in  range(4):
        if my[i] == their[i]:
            bulls += 1
        else:
            if their[i] in my:
                cows += 1
    return bulls, cows


def toList(x):
    l = []
    while not x == 0:
        l.append(x % 10)
        x //= 10
    return l[::-1]

def toNumber(l):
    x = 0
    for i in range(len(l)):
        x *= 10
        x += l[i]
    return x

def correctList(l):
    s = {i for i in l}
    return len(s) == 4

def myGuess():
    if len(myGuess.options) == 0:
        return -1
    seed()
    i = randint(0, len(myGuess.options) - 1)
    guess = copy(myGuess.options[i])
    print("My quess:", end = "\n\t")
    print(toNumber(guess))
    log.write("My quess: " + str(toNumber(guess)) + "; your answer: ")
    submit = 0
    print("Your answer:")
    while not submit:
        try:
            print("\tBulls:", end = " ")
            b = int(input())
            # 112358 - key to stop the game
            if b == 112358:
              return -2
            print("\tCows:", end = " ")
            c = int(input())
            if c == 112358:
                return -2
            if b < 0 or c < 0 or b + c > 4 or (b == 3 and c != 0):
                raise Exception("wrong answer: "+ str(b) + " " + str(c))
            submit = 1
        except Exception as e:
            print("Something wrong... Try again.")
            log.write("\n\nException: " + str(e)+ "\n\n")
    log.write(str(b)+ " " + str(c) + "\n")
    if b == 4: return 0
    if myGuess.level == -1:
        level_rand = 1
    for i in range(len(myGuess.options)):
        rb, rc = calculate(myGuess.options[i], guess)
        # With higher level the probability of zero level_rand is lower
        if myGuess.level != -1: 
            level_rand = randint(0, 99) % (level + 1)
        if (rb != b or rc != c) and level_rand != 0:
            myGuess.options[i].clear()
    myGuess.options = [l for l in myGuess.options if not len(l) == 0]
    return 1
    
def yourGuess():
    print("Your guess:")
    log.write("Your guess: ")
    submit = 0
    while not submit:
        try:
            print("", end ="\t")
            guess = int(input())
            if guess == 112358:
                return -2
            if not correctList(toList(guess)):
                raise Exception("invalid guess: " + str(guess))
            submit = 1
        except Exception as e:
            print("Something wrong... Try again.")
            log.write("\n\nException: " + str(e)+ "\n\n")
    log.write(str(guess) + "; my answer: ")
    b, c = calculate(yourGuess.secret, toList(guess))
    log.write(str(b)+ " " + str(c) + "\n")
    print("My answer:")
    print("\tBulls:", b)
    print("\tCows:", c)
    if b == 4: return 0
    else:
        return 1
    
myGuess.options = []
for i in range(1, 10):
    for j in range(10):
        if i == j:
            continue
        for k in range(10):
            if k == i or k == j:
                continue
            for l in range(10):
                if l == i or l == j or l == k:
                    continue
                myGuess.options.append([i, j, k, l])


print("Hello! Let's start.")
print("Choose level from 1 to 10:")
level = int(input())
if not level in range(1, 11):
    print("Something wrong... Now you have to play with the smartest bot!")
    level = -1
myGuess.level = level

seed()
ind = randint(0, len(myGuess.options) - 1)
yourGuess.secret = copy(myGuess.options[ind])

for i in range(100):
    sec.write("SECRET!SECRET!SECRET!SECRET!SECRET!\n")
sec.write(str(toNumber(yourGuess.secret)))
sec.close()

myRes = 1
yourRes = 1
while myRes and yourRes:
    try:
        print("****************")
        myRes = myGuess()
        print("----------------")
        if myRes == -1:
            print("I don't know. I have no more options.")
            break
        if myRes == -2:
            break
        yourRes = yourGuess()
        print("****************")
        if yourRes == -2:
            break
    except Exception as e:
        log.write("\n\nUnexpected exception: " + str(e)+ "\n\n")
        break
log.close()
if myRes == 0 and yourRes == 0:
    print("Draw! Maybe one more time?")
elif myRes == 0:
    print("I'm smarter than you! :)")
    print("My number was:", str(toNumber(yourGuess.secret)))
elif yourRes == 0:
    print("You're smarter than me! :(")



        

 

    
