/*===========================================================================
dirsize
Copyright (c)2015-2017 Kevin Boone
Distributed under the terms of the GPL, v3.0

Note -- support for following symlinks is not implemented yet
(and probably is not even useful)
*===========================================================================*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

#ifndef BOOL
typedef int BOOL;
#endif
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

/*======================================================================
  print_file_size 
======================================================================*/
void print_file_size (int64_t l, BOOL format_numbers)
  {
  static const int64_t K = 1024;
  static const int64_t M = 1024*1024;
  static const int64_t G = 1024*1024*1024;
  if (format_numbers)
    {
    if (l < K)
      printf ("%5lld", (long long) l);
    else if (l < M)
      printf ("%4lldk", (long long) l / K);
    else if (l < G)
      printf ("%4lldM", (long long) l / M);
    else 
      printf ("%4lldG", (long long) l / G);
    }
  else
    printf ("%9lld", (long long) l);
  }

/*======================================================================
  do_dir 
======================================================================*/
void do_dir (const char *dir, BOOL hidden, BOOL dir_only, BOOL deref_links,
             int depth, int report_depth, 
             BOOL format_numbers, int64_t *num_files, int64_t* num_dirs,
             int64_t *file_size)
  {
  *num_files = *num_dirs = *file_size = 0;

  struct stat sb;
  if (lstat (dir, &sb) != 0)
    {
    fprintf (stderr, "%s: %s\n", dir, strerror (errno));
    return;
    }
  if (S_ISLNK (sb.st_mode)) 
    {
    if (!deref_links) return;
    }

  int64_t _num_files = 0;
  int64_t _num_dirs = 0;
  int64_t _file_size = 0;

  DIR *d = opendir (dir);
  if (d)
    {
    struct dirent *de; 
    while ((de = readdir (d)))
      {
      char *path = malloc (strlen (dir) + 2 + strlen (de->d_name));
      strcpy (path, dir);
      if (path[strlen(path)-1] != '/')
        strcat (path, "/");
      strcat (path, de->d_name);
      struct stat sb;
      if (lstat (path, &sb) == 0)
        {
        if (strcmp (de->d_name, ".") == 0)
          continue;
        if (strcmp (de->d_name, "..") == 0)
          continue;
        if (de->d_name[0] != '.' || hidden)
          {
          if (S_ISLNK (sb.st_mode)) 
            {
            if (!deref_links) continue;
            // readlink here
            // TODO
            }
          if (S_ISREG (sb.st_mode))
            {
            _file_size += sb.st_size;
            }
          else if (S_ISDIR (sb.st_mode))
            {
            if (!dir_only)
              {
              int64_t __num_files;
              int64_t __num_dirs;
              int64_t __file_size;
              do_dir (path, hidden, dir_only, deref_links, 
               depth + 1, report_depth, 
               format_numbers, 
               &__num_files, &__num_dirs, &__file_size);
              _num_files += __num_files;
              _num_dirs += __num_dirs;
              _file_size += __file_size;
              }
            }
          }
        }
      else
        {
        fprintf (stderr, "%s: %s\n", path, strerror (errno));
        }
      free (path);
      }; 

    closedir (d);
    }
  else
    {
    fprintf (stderr, "%s: %s\n", dir, strerror (errno));
    }

  *file_size += _file_size;
  *num_dirs += _num_dirs;
  *num_files += _num_files;
  if (depth <= report_depth  || report_depth < 0)
    {
    print_file_size (_file_size, format_numbers);
    printf (" %s\n", dir);
    }
  }


/*======================================================================
  show_version
======================================================================*/
void show_version (FILE *f, const char *argv0)
  {
  fprintf (f, "%s version " VERSION "\n", argv0); 
  fprintf (f, "Totals true file sizes in a directory.\n");
  fprintf (f, "Copyright (c)2015-2017 Kevin Boone; distributed under the\n");
  fprintf (f, "terms of the GNU Public Licence, v3.0.\n");
  }

/*======================================================================
  show_short_usage 
======================================================================*/
void show_short_usage (FILE *f, const char *argv0)
  {
  fprintf (f, "Usage: %s [-adfhsv] [directory]\n", argv0);
  }

/*======================================================================
  show_long_usage 
======================================================================*/
void show_long_usage (FILE *f, const char *argv0)
  {
  fprintf (f, "Usage: %s [-adfhsv] [directory]\n", argv0);
  fprintf (f, "  -a           : include hidden files\n");
  fprintf (f, "  -d           : directory only: do not recurse\n");
  //fprintf (f, "  -l           : follow symbolic links\n");
  fprintf (f, "  -f           : format numbers as kB, Mb, etc\n");
  fprintf (f, "  -h           : show this message\n");
  fprintf (f, 
    "  -s           : summarize: just show figures for top directories\n");
  fprintf (f, "  -v           : show version information\n");
  fprintf (f, "For more information, "
       "see http://kevinboone.net/README_dirsize.html\n");  
  }

/*======================================================================
  show_version
======================================================================*/
int main (int argc, char **argv)
  {
  int c;
  BOOL hidden = FALSE;
  BOOL dir_only = FALSE;
  BOOL deref_links = FALSE;
  BOOL format_numbers = FALSE;
  int report_depth = -1; // -1 = show all directories 

  while ((c = getopt (argc, argv, "adfhsv")) !=-1)
    {
    switch (c)
      {
      case 'a':
        hidden = TRUE;
        break;
      case 'd':
        dir_only = TRUE;
        break;
      case 'f':
        format_numbers = TRUE;
        break;
      case 'l':
        deref_links = TRUE;
        break;
      case 'h':
        show_long_usage (stdout, argv[0]);
        exit (0);
      case 's':
        report_depth = 1; 
        break;
      case 'v':
        show_version (stdout, argv[0]);
        exit(0);
      default:
        exit(-1);
      }
    }

  int64_t num_files = 0;
  int64_t num_dirs = 0;
  int64_t file_size = 0;
  int i;

  if (optind == argc)
    {
    int64_t _num_files;
    int64_t _num_dirs;
    int64_t _file_size;
    do_dir (".", hidden, dir_only, deref_links, 
        0, report_depth, format_numbers, &_num_files, &_num_dirs, &_file_size);
    }
  else
    {
    for (i = optind; i < argc; i++)
      {
      int64_t _num_files;
      int64_t _num_dirs;
      int64_t _file_size;
      do_dir (argv[i], hidden, dir_only, deref_links, 
        0, report_depth, format_numbers, &_num_files, &_num_dirs, &_file_size);
      num_files += _num_files;
      num_dirs += _num_dirs;
      file_size += _file_size;
      }
    }

  if (argc - optind > 1)
    {
    print_file_size (file_size, format_numbers);
    printf (" %s\n", "(total)");
    }

  return 0;
  }


