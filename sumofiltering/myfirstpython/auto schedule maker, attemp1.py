import findtime
import random

sessions = ["math", "physics", "arabic", "english"]

session1start = 455
session1end = 505
session2start = 505
session2end = 555
session3start = 555
session3end = 605
session4start = 605
session4end = 655
teacher1 = None
while teacher1 not in sessions:
    teacher1 = str(input("what teacher are you?: "))
teacher1come = findtime.findrealtime(str(input("when do you come: ")))
teacher1leave = findtime.findrealtime(str(input("when do you leave: ")))
teacher2 = None
while teacher2 not in sessions:
    teacher2 = str(input("what teacher are you?: "))
teacher2come = findtime.findrealtime(str(input("when do you come: ")))
teacher2leave = findtime.findrealtime(str(input("when do you leave: ")))
teacher3 = None
while teacher3 not in sessions:
    teacher3 = str(input("what teacher are you?: "))
teacher3come = findtime.findrealtime(str(input("when do you come: ")))
teacher3leave = findtime.findrealtime(str(input("when do you leave: ")))
teacher4 = None
while teacher4 not in sessions:
    teacher4 = str(input("what teacher are you?: "))
teacher4come = findtime.findrealtime(str(input("when do you come: ")))
teacher4leave = findtime.findrealtime(str(input("when do you leave: ")))

def check_teacher_attendace(teachercome, teachergo, teacher):
    classes_the_teacher_would_attend = ()
    if teachercome <= session1start:
        if teachergo >= session1end:
            print(f"the {teacher} teacher can attend the first class")
            classes_the_teacher_would_attend.append(teacher)
    if teachercome <= session2start:
        if teachergo >= session2end:
            print(f"the {teacher} teacher can attend the second class")
            classes_the_teacher_would_attend.append(teacher)
    if teachercome <= session3start:
        if teachergo >= session3end:
            print(f"the {teacher} teacher can attend the third class")
            classes_the_teacher_would_attend.append(teacher)
    if teachercome <= session4start:
        if teachergo >= session4end:
            print(f"the {teacher}teacher can attend the fourth class")
            classes_the_teacher_would_attend.append(teacher)


print(f"for the {teacher1} teacher")
check_teacher_attendace(teacher1come, teacher1leave, teacher1)
print()
print(f"for the {teacher2} teacher")
check_teacher_attendace(teacher2come, teacher2leave, teacher2)
print()
print(f"for the {teacher3} teacher")
check_teacher_attendace(teacher3come, teacher3leave, teacher3)
print()
print(f"for the {teacher4} teacher")
check_teacher_attendace(teacher4come, teacher4leave, teacher4)
