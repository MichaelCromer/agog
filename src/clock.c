#include "clock.h"


int agog_clockon(int argc, char *argv[]) 
{
  if (argc < 3) {
    printf("Error: Missing project name\n");
    return EXIT_FAILURE;
  }

  char *project_name = argv[2];

  // first check if the project exists
  if (!project_exists(project_name)) {
    printf("Error: Project %s does not exist.\n", project_name);
    return EXIT_FAILURE;
  }

  // then check if we are already clocked on to a project
  if (is_clocked_on()) {
    system("rm " AGOG_CURRENTS_DIR "/project");
  }

  char project_path[256];
  snprintf(project_path, sizeof(project_path), "%s/%s", AGOG_PROJECTS_DIR, project_name);

  char command[512];
  snprintf(command, sizeof(command), "ln -s %s " AGOG_CURRENTS_DIR "/project", project_path);

  int status = system(command);
  if (status != 0) {
    printf("Error: Could not clock on to project %s.\n", project_name);
    exit(EXIT_FAILURE);
  }

  printf("Clocked on to project %s\n", project_name);
  return EXIT_SUCCESS;
}


int agog_clockoff() 
{
  if (!is_clocked_on()) {
    printf("Error: Not currently clocked on to any project.\n");
    return EXIT_FAILURE;
  }

  printf("Clocking off project %s\n", get_current_project());
  int status = system("rm " AGOG_CURRENTS_DIR "/*");
  if (status != 0) {
    printf("Error: Could not clock off.\n");
    exit(EXIT_FAILURE);
  }
  return EXIT_SUCCESS;
}

bool is_clocked_on() 
{
  return (system("test -L " AGOG_CURRENTS_DIR "/project") == 0);
}


// expected format: xhym or xh or xm
// output: time in minutes
int parse_time(char *time_str)
{
  int minutes = 0;
  int hours = 0;
  int unit = 0;

  int h_count = 0;
  int m_count = 0;

  for (char *c = time_str; *c; c++) {
    if (isdigit(*c)) {
      unit = unit * 10 + (*c - '0');
    } else if (*c == 'h') {
      h_count++;
      hours = unit;
      unit = 0;
    } else if (*c == 'm') {
      m_count++;
      minutes = unit;
      unit = 0;
    } else {
      printf("Error: Invalid time format. Use XhYm or Xh or Xm\n");
      exit(EXIT_FAILURE);
    }
  }

  if (h_count > 1 || m_count > 1) {
    printf("Error: Invalid time format. Use XhYm or Xh or Xm\n");
    exit(EXIT_FAILURE);
  }

  return hours * 60 + minutes;
}
