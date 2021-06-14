import sys

class_name = sys.argv[1]
path = sys.argv[2]

if(path[-1] == "/"):
    path = path[0:-1]

h_file_path = path+"/{name}.h".format(name=class_name)
cpp_file_path = path+"/{name}.cpp".format(name=class_name)

def content_from_file(file_path):
    f_template = open(file_path, 'r')
    content = f_template.readlines()
    f_template.close()
    return ''.join([line for line in content])

def write_content_to_file(content, file_path):
    f = open(file_path, 'w')
    f.write(content)
    f.close()

# create h file
h_file_content_format = content_from_file("../Templates/Class/ClassTemplate.h")
h_file_content = h_file_content_format % class_name
write_content_to_file(h_file_content, h_file_path)

# create cpp file
cpp_file_content_format = content_from_file("../Templates/Class/ClassTemplate.cpp")
cpp_file_content = cpp_file_content_format % class_name
write_content_to_file(cpp_file_content, cpp_file_path)