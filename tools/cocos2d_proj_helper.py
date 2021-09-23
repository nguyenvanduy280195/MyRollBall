import tkinter as tk
import tkinter.filedialog as filedialog
import tkinter.messagebox as messagebox
import configparser
import os

config = configparser.ConfigParser()
config.read("config.ini")

def content_from_file(file_path):
    f_template = open(file_path, 'r')
    content = f_template.readlines()
    f_template.close()
    return ''.join([line for line in content])

def write_content_to_file(content, file_path):
    f = open(file_path, 'w')
    f.write(content)
    f.close()

def create_a_new_class(class_name, saved_path):
    if(saved_path[-1] == "/"):
        saved_path = saved_path[0:-1]

    h_file_path = saved_path+"/{name}.h".format(name=class_name)
    cpp_file_path = saved_path+"/{name}.cpp".format(name=class_name)

    # create h file
    h_file_content_format = content_from_file(config['FOLDER']['root'] + config['FOLDER']['template.class'] + config['FILE']['h'])
    h_file_content = h_file_content_format % class_name
    write_content_to_file(h_file_content, h_file_path)

    # create cpp file
    cpp_file_content_format = content_from_file(config['FOLDER']['root'] + config['FOLDER']['template.class'] + config['FILE']['cpp'])
    cpp_file_content = cpp_file_content_format % class_name
    write_content_to_file(cpp_file_content, cpp_file_path)

class CMakelistHelper:
    def __init__(self):
        self.cmakelist_path = config['FOLDER']['root'] + config['FILE']['cmakelists']
        self.h_list_file, self.cpp_list_file = self.list_files()
        self.cmakelist = self.get_content_from_file(self.cmakelist_path)
        self.cmakelist_new = []

    def get_content_from_file(self, file_path):
        f_template = open(file_path, 'r')
        content = f_template.readlines()
        f_template.close()
        return content

    def write_content_to_CMakeList_file(self, content):
        with open(self.cmakelist_path, 'w') as f:
            f.writelines(content)
            f.close()

    def list_files(self):

        cpp_file_set = []
        h_file_set = []

        # list file
        for dir_, _, files in os.walk(config['FOLDER']['root'] + config['FOLDER']['classes']):
            for file_name in files:
                rel_dir = os.path.relpath(dir_, config['FOLDER']['root'])
                rel_file = os.path.join(rel_dir, file_name)
                rel_file = rel_file.replace("\\", "/")
                if file_name[-1] == 'h':
                    h_file_set.append(rel_file)
                else:
                    cpp_file_set.append(rel_file)
        return h_file_set, cpp_file_set

    def append_h_cpp_inl_files_to_cmakelists(self):
        cpp_writed = False
        h_writed = False
        for line in self.cmakelist:
            # Classes/Scenes/MainMenuScene.h in CMakeLists.txt -> skip
            # proj.android/app/jni/hellocpp/main.h -> not skip
            if "Classes" in line and ".h" in line:
                continue

            # Classes/Scenes/MainMenuScene.cpp in CMakeLists.txt -> skip
            # proj.android/app/jni/hellocpp/main.cpp -> not skip
            if "Classes" in line and (".cpp" in line or ".inl" in line):
                continue

            self.cmakelist_new.append(line)

            # Writting cpp after this line
            if not cpp_writed and "list(APPEND GAME_SOURCE" in line:
                cpp_writed = True
                for l in self.cpp_list_file:
                    self.cmakelist_new.append(l + '\n')

            # Writting cpp after this line
            if not h_writed and "list(APPEND GAME_HEADER" in line:
                h_writed = True
                for l in self.h_list_file:
                    self.cmakelist_new.append(l + '\n')

window = tk.Tk(className="Duy's cocos2d-x tools")
window.resizable(0, 0)
window.geometry("1000x200+300+300")

frame_input = tk.Frame(master=window)
frame_input.pack(fill=tk.X, side=tk.TOP)

frame_output = tk.Frame(master=window)

frame_output.columnconfigure([0,1,2], weight=1)
frame_output.pack(fill=tk.X, side=tk.BOTTOM)

font_default = 'Consolas 15'

# Create classname

lbl_classname = tk.Label(master=frame_input, text="Class name", font=font_default)
# lbl_classname.grid(row=0, column=0)
lbl_classname.pack(side=tk.TOP, anchor=tk.NW)

entry_classname = tk.Entry(master=frame_input, font=font_default)
#entry_classname.grid(row=1,column=0)
entry_classname.pack(side=tk.TOP, anchor=tk.NW, fill=tk.X)

# Create saved path

lbl_saved_path = tk.Label(master=frame_input, text="Stored Path", font=font_default)
#lbl_saved_path.grid(row=2, column=0)
lbl_saved_path.pack(side=tk.TOP, anchor=tk.NW)

entry_saved_path = tk.Entry(master=frame_input, font=font_default)
#entry_saved_path.grid(row=3,column=0)
entry_saved_path.pack(side=tk.TOP, anchor=tk.NW, fill=tk.X)

def on_entry_saved_path_click(event):
    saved_path = filedialog.askdirectory(initialdir=config['FOLDER']['root'] + config['FOLDER']['classes'])
    entry_saved_path.insert(0, saved_path)
    pass

entry_saved_path.bind("<1>", on_entry_saved_path_click)


# Create Add button
def on_btn_add_click():
    classname = entry_classname.get()
    if classname == "":
        messagebox.showerror("Error", "Class name must not empty")
        return

    savedpath = entry_saved_path.get()
    if savedpath == "":
        messagebox.showerror("Error", "Saved path must not empty")
        return
    
    create_a_new_class(classname, savedpath)
    messagebox.showinfo("Info", "Creating {} class success".format(classname))

btn_add = tk.Button(master=frame_output, text="Add", command=on_btn_add_click, font=font_default)
btn_add.grid(row=0, column=0, sticky="nsew", padx=5, pady=5)

# Create Build CMakelist button
def on_btn_build_click():
    cmakelist_helper = CMakelistHelper()
    cmakelist_helper.append_h_cpp_inl_files_to_cmakelists()
    cmakelist_helper.write_content_to_CMakeList_file(cmakelist_helper.cmakelist_new)

    os.system('cd {} && cmake ..'.format(config['FOLDER']['root'] + config['FOLDER']['proj.win32']))
    messagebox.showinfo("Info", 'Build success')

btn_build = tk.Button(master=frame_output, text="Build CMakelist", command=on_btn_build_click, font=font_default)
btn_build.grid(row=0, column=1, sticky="nsew", padx=5, pady=5)

# Create Open VS2019 Proj button
btn_open_vs2019_project = tk.Button(master=frame_output, text="Open VS2019 Project",
                                    font=font_default, 
                                    command=lambda : os.system('start {}'.format(config['FOLDER']['root'] + config['FOLDER']['proj.win32'] + config['FILE']['vs2019solution'])))
btn_open_vs2019_project.grid(row=0, column=2, sticky="nsew", padx=5, pady=5)

window.mainloop()
