import os, sys

solution_name = 'qt_shlee'
path = '.'

base_cmake_text = '''cmake_minimum_required(VERSION 3.0)
project({0} CXX)
set(TARGET {1})

if(UNIX)
  message("Unix configuration")

elseif("WIN32")
  message("Windows configuration")
  set(CMAKE_CXX_FLAGS "/EHa")

endif()


########################################################

# CMake policies
cmake_policy(SET CMP0015 NEW)
'''

cmake_text = '''cmake_minimum_required(VERSION 3.0)
project({0} CXX)
set(TARGET {1})

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5PrintSupport REQUIRED)


file(GLOB project_SRCS *.cpp *.h *.qml *.ts)

include_directories(${{PROJECT_SOURCE_DIR}})

set(target_qml "${{PROJECT_NAME}}_qml")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

add_executable({1} ${{project_SRCS}})

target_link_libraries({1} Qt5::Core Qt5::Widgets {2})
'''

def create_cmake(path, project):
    '''
    :param path: path to the solution
    :param project: project name
    '''
    project_path = os.path.join(path, project)
    f = open(os.path.join(project_path, 'CMakeLists.txt'), 'w')
    if project == 'ch_10_text_edit':
        f.write(cmake_text.format(project, project, "Qt5::PrintSupport"))
    else:
        f.write(cmake_text.format(project, project, ""))
    f.close()


def create_base_cmake(root_path, solution_name, projects_list):
    '''
    :param root_path: path to the solution
    :param solution_name: string name of the solution
    :param projects_list: list of projects (subdirectory names)
    '''
    f = open(os.path.join(root_path, 'CMakeLists.txt'), 'w')
    f.write(base_cmake_text.format(solution_name, solution_name))
    for project in projects_list:
        f.write('add_subdirectory({0})\n'.format(project))
    f.close()

def mass_create_cmake(root_path, solution_name):
    '''
    :param root_path: path to the solution
    :param solution_name: string name of the solution
    '''
    projects_list = []
    for project in os.listdir(root_path):
        if os.path.isdir(project):
                create_cmake(root_path, project)
                projects_list.append(project)

    create_base_cmake(root_path, solution_name, projects_list)


def main(argv):
    try:
        mass_create_cmake(path, solution_name)
        return os.system('cmake . -DCMAKE_PREFIX_PATH=C:/Qt/5.5/msvc2013')
    except IndexError:
        print("Usage: python create_cmake.py <solution_name> <solution_path>")
#    except Exception as e:
#        print('Exception: {0}'.format(e))
    # return non-zero if here
    return 1

###################################
if __name__ == "__main__":
    sys.exit(main(sys.argv))
