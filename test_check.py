import sys;

if __name__ == "__main__":
    name = sys.argv[1]
    name = name.split("_")

    if not name[0].isdigit():
        exit(1)
    err_code = eval(name[0])
    code = eval(sys.argv[2])

    #print(code)
    #print(err_code)
    if code != err_code:
        exit(2)
    if code == 0:
        exit(0)

    if len(name) < 2:
        exit(1)
    if not name[1].isdigit():
        exit(1)
    err_line = eval(name[1])

    line = sys.argv[3]
    line = eval(line.split()[1].split(':')[0])
    #print(line)
    #print(err_line)
    if line != err_line:
        exit(3)
    #print("success")
    exit(0)
