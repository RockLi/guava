/*
 * Copyright 2015 The guava Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "guava_server.h"
#include "guava_conn.h"
#include "guava_router/guava_router.h"
#include "guava_module.h"
#include "guava_module_router.h"
#include "guava_memory.h"

guava_server_t *guava_server_new() {
  guava_server_t *server = (guava_server_t *)guava_calloc(1, sizeof(guava_server_t));
  if (!server) {
    return NULL;
  }

  server->routers = NULL;
  server->middlewares = NULL;
  server->debug = GUAVA_FALSE;

  return server;
}

void guava_server_free(guava_server_t *server) {
  Py_XDECREF(server->routers);
  Py_XDECREF(server->middlewares);
  guava_free(server);
}

void guava_server_on_conn(uv_stream_t *stream, int status) {
  guava_conn_t *conn = guava_conn_new();

  http_parser_init(&conn->parser, HTTP_REQUEST);

  guava_server_t *server = (guava_server_t *)stream->data;

  conn->server = server;

  uv_tcp_init(&server->loop, &conn->stream);

  conn->parser.data = conn;
  conn->stream.data = conn;

  uv_accept(stream, (uv_stream_t *)&conn->stream);
  uv_read_start((uv_stream_t *)&conn->stream, guava_server_on_alloc, guava_server_on_read);
}

void guava_server_on_alloc(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
  *buf = uv_buf_init((char *)guava_malloc(suggested_size), (unsigned int)suggested_size);
}

void guava_server_on_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf) {
  guava_conn_t *conn = (guava_conn_t *)stream->data;
  if (nread < 0 || nread == UV_EOF) {
    uv_close((uv_handle_t *)&conn->stream, guava_server_on_close);
  } else if (nread > 0) {
    if(http_parser_execute(&conn->parser, &conn->parser_settings, buf->base, nread) != (size_t)nread) {
      fprintf(stderr, "400\n");
    }
  }

  if (buf->base) {
    guava_free(buf->base);
  }
}

void guava_server_on_close(uv_handle_t *handle) {
  guava_conn_t *conn = (guava_conn_t *)handle->data;
  guava_conn_free(conn);
}

void signal_shutdown_cb(uv_signal_t *handle, int signum) {
  fprintf(stderr, "Caught signal, ready to shutdown the web server\n");

  guava_server_t *server = (guava_server_t *)handle->data;

  uv_loop_close(&server->loop);

  uv_signal_stop(&server->signal);

  exit(0);
}

void guava_server_start(guava_server_t *server, const char *ip, uint16_t port, int backlog) {
  if (!server->routers) {
    /* StaticRouter *static_router = (StaticRouter *)PyObject_New(StaticRouter, &StaticRouterType); */
    /* static_router->router.router = (guava_router_t *)guava_router_static_new(); */
    /* guava_router_set_mount_point((guava_router_t *)static_router->router.router, "/"); */
    /* guava_router_static_set_directory((guava_router_static_t *)static_router->router.router, "."); */
    /* guava_router_static_set_allow_index((guava_router_static_t *)static_router->router.router, GUAVA_TRUE); */
    /* guava_server_add_router(server, (Router *)static_router); */
  }

  fprintf(stdout, "Listening on %s:%d...\n", ip, port);

  uv_loop_init(&server->loop);
  uv_tcp_init(&server->loop, &server->server);

  struct sockaddr_in address;

  uv_ip4_addr(ip, port, &address);

  uv_tcp_bind(&server->server, (const struct sockaddr *)&address, 0);

  server->server.data = server;

  uv_signal_init(&server->loop, &server->signal);
  uv_signal_start(&server->signal, signal_shutdown_cb, SIGINT);
  server->signal.data = server;

  uv_listen((uv_stream_t *)&server->server, backlog, guava_server_on_conn);

  /* Py_BEGIN_ALLOW_THREADS */
  uv_run(&server->loop, UV_RUN_DEFAULT);
  /* Py_END_ALLOW_THREADS */
}

void guava_server_add_router(guava_server_t *server, Router *router) {
  if (!server || !router) {
    return;
  }

  if (!server->routers) {
    server->routers = PyList_New(0);
  }

  PyList_Append(server->routers, (PyObject *)router);
}
