import sys
import time
import subprocess


# Pause between service stop and start
RestartPause = 5


def check_output(command):
    '''
    Execute command passed as parameter.
    :return : output as a tuple (stdout_data, stderr_data)
    :rtype : tuple (string, string)
    :raise subprocess.CalledProcessError: if process return code is not 0
    '''
    process = subprocess.Popen(command,
                               shell=True,
                               stdout=subprocess.PIPE,
                               stderr=subprocess.STDOUT,
                               universal_newlines=True)
    output = process.communicate()
    retcode = process.poll()
    if retcode:
            raise subprocess.CalledProcessError(retcode, command, output=output[0])
    return output


def start_tc_service(agent_name):
    '''
    Start TeamCity service without check
    :raise subprocess.CalledProcessError: if process return code is not 0
    '''
    start_command = r'sc \\{0} start TCBuildAgent'.format(agent_name)
    check_output(start_command)


def stop_tc_service(agent_name):
    '''
    Stop TeamCity service without check
    :raise subprocess.CalledProcessError: if process return code is not 0
    '''
    stop_command = r'sc \\{0} stop TCBuildAgent'.format(agent_name)
    check_output(stop_command)


def is_service_running(agent_name):
    '''
    :param agent_name: TC agent name without \\ (e.g. 'longmasu0032')
    :return : True is TC service on the 'agent_name' agent is running, False otherwise
    :raise WindowsError: if output from Service Control does not contain RUNNING or STOPPED string
    '''
    query_command = r'sc \\{0} query TCBuildAgent'.format(agent_name)
    query_service = check_output(query_command)
    if 'RUNNING' in query_service[0]:
        return True
    if 'STOPPED' in query_service[0]:
        return False
    raise WindowsError('Unrecognized output from Service Control')


###########################################################################
def restart_agent(agent_name):
    '''
    Restart agent if started
    Start agent if stopped
    '''
    if is_service_running(agent_name):
        stop_tc_service(agent_name)
        time.sleep(RestartPause)
        start_tc_service(agent_name)
        print('{0} restarted successfully'.format(agent_name))
    else:
        print('{0} has not been started. Start it'.format(agent_name))
        start_tc_service(agent_name)


def restart_new():
    '''
    Restart TC service on Win2008 machines
    Check server names exist, they could be out-of-date
    '''
    # range means [31,39)
    for agent_num in range(31, 39):
        restart_agent('longmasu00{0}'.format(agent_num))


def restart_old():
    '''
    Restart TC service on Win2003 machines
    Check server names exist, they could be out-of-date
    '''
    for agent_num in [52, 54, 56, 57, 58]:
        restart_agent('lonfxrmsvsu{0}'.format(agent_num))


def main():
    try:
        restart_new()
        #restart_old()
    except subprocess.CalledProcessError as sub:
        print sub
    except WindowsError as win:
        print win
    except RuntimeError as err:
        print err

###########################################################################
if __name__ == "__main__":
    sys.exit(main())
