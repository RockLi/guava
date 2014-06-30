/*
 * Copyright 2014 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava_handler.h"
#include "guava_response.h"
#include "guava_string.h"
#include "guava_router/guava_router.h"
#include "guava_mime_type.h"
#include "guava_session/guava_session.h"

#if defined(__APPLE__)
extern int uv___stream_fd(const uv_stream_t* handle);
#else
#define uv___stream_fd(handle) ((handle)->io_watcher.fd)
#endif

#define GUAVA_LIBUV_GET_STREAM_FD uv___stream_fd

void guava_handler_static(guava_router_t *router,
                          guava_conn_t *conn,
                          guava_request_t *req,
                          guava_response_t *resp,
                          uv_write_cb on_write,
                          uv_fs_cb on_sendfile) {

  char *req_filename = strstr(req->path, "/");

  char filename[MAXPATH];
  snprintf(filename, sizeof(filename), "%s/%s", ".", req_filename);
  uv_fs_t stat_req;
  uv_fs_stat(&conn->server->loop, &stat_req, filename, NULL);

  uv_fs_req_cleanup(&stat_req);

  do {
    if (stat_req.result != 0) {
      guava_response_404(resp, NULL);
      guava_response_send(resp, on_write);
      break;
    }

    uv_stat_t *s = &stat_req.statbuf;
    guava_response_set_status_code(resp, 200);

    if (S_ISDIR(s->st_mode)) {
      guava_response_set_header(resp, "Content-Type", "text/html");
      guava_string_t data = guava_string_new("");

      uv_fs_t readdir_req;
      readdir_req.data = resp;
      uv_fs_readdir(&conn->server->loop, &readdir_req, filename, 0, NULL);

      const char *ptr = readdir_req.ptr;
      size_t idx = 0;
      while (idx < readdir_req.result) {
        data = guava_string_append_raw(data, "<a href=\"");
        data = guava_string_append(data, req->path);
        if (req->path[guava_string_len(req->path)-1] != '/') {
          data = guava_string_append_raw(data, "/");
        }
        data = guava_string_append_raw(data, ptr);
        data = guava_string_append_raw(data, "\">");
        data = guava_string_append_raw(data, ptr);
        data = guava_string_append_raw(data, "</a><br />");
        ptr += strlen(ptr) + 1;
        ++idx;
      }
      guava_response_set_data(resp, data);
      uv_fs_req_cleanup(&readdir_req);
    } else {
      guava_response_set_header(resp, "Content-Type", guava_mime_type_guess(filename));
      char buf[1024];
      snprintf(buf, sizeof(buf), "%zd", s->st_size);
      guava_response_set_header(resp, "Content-Length", buf);
    }

    guava_response_send(resp, on_write);

    if (!S_ISDIR(s->st_mode)) {
      uv_fs_t open_req;
      int fd = uv_fs_open(&conn->server->loop, &open_req, filename, O_RDONLY, 0, NULL);
      uv_fs_t *write_req = (uv_fs_t *)malloc(sizeof(*write_req));
      uv_fs_sendfile(&conn->server->loop, write_req, GUAVA_LIBUV_GET_STREAM_FD((uv_stream_t *)&conn->stream), fd, 0, (size_t)s->st_size, on_sendfile);
    }
  } while(0);
}
