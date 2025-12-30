
def count_numbers():
        try:
            start = int(input("Enter the starting number: "))
            end = int(input("Enter the ending number: "))

            if start <= end:
                print(f"Counting from {start} to {end}:")
                for i in range(start, end + 1):
                    print(i)
            else:
                print("The starting number should be less than or equal to the ending number.")
                count_numbers()
        except ValueError:
            print("you can't input letters you idiot")
            count_numbers()
count_numbers()

