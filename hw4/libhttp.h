/*
 * A simple HTTP library.
 *
 * Usage example:
 *
 *     // Returns NULL if an error was encountered.
 *     struct http_request *request = http_request_parse(fd);
 *
 *     ...
 *
 *     http_start_response(fd, 200);
 *     http_send_header(fd, "Content-type", http_get_mime_type("index.html"));
 *     http_send_header(fd, "Server", "httpserver/1.0");
 *     http_end_headers(fd);
 *     http_send_string(fd, "<html><body><a href='/'>Home</a></body></html>");
 *
 *     close(fd);
 */

#ifndef LIBHTTP_H
#define LIBHTTP_H

/*
 * Functions for parsing an HTTP request.
 */
struct http_request {
  char *method;
  char *path;
};

struct http_request *http_request_parse(int fd);

/*
 * Functions for sending an HTTP response.
 */
void http_start_response(int fd, int status_code);
void http_send_header(int fd, char *key, char *value);
void http_end_headers(int fd);
void http_format_href(char *buffer, char *path, char *filename);
void http_format_index(char *buffer, char *path);

/*
 * Helper function: gets the Content-Type based on a file name.
 */
char *http_get_mime_type(char *file_name);

#endif
