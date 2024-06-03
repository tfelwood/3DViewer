/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfelwood <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/05 15:10:46 by tfelwood          #+#    #+#             */
/*   Updated: 2021/12/06 22:42:34 by tfelwood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static void ft_buf_free(t_buffer *buf, t_buffer **buf_list) {
  if (buf && buf_list && *buf_list) {
    free(buf->data);
    if (buf == *buf_list) *buf_list = buf->next;
    if (buf->prev) buf->prev->next = buf->next;
    if (buf->next) buf->next->prev = buf->prev;
    free(buf);
  }
}

static t_buffer **get_buffer_list() {
  static t_buffer *buflist = NULL;

  return (&buflist);
}

static t_buffer *ft_find_fd(t_buffer **lst, int fd) {
  t_buffer *tmp;
  t_buffer *elem;

  tmp = *lst;
  if (tmp) {
    while (tmp->fd != fd && tmp->next != NULL) tmp = tmp->next;
    if (tmp->fd == fd) return (tmp);
  }
  elem = ft_create_buf(tmp, fd);
  if (!(*lst))
    *lst = elem;
  else
    tmp->next = elem;
  return (elem);
}

char *get_next_line(int fd) {
  char *new_line;
  t_buffer *buf;
  char is_str;

  new_line = NULL;
  is_str = 0;
  if (BUFFER_SIZE > 0 && fd >= 0) {
    buf = ft_find_fd(get_buffer_list(), fd);
    while (!is_str && buf) {
      if (!ft_buf_update(buf)) break;
      new_line = ft_strjoin(new_line, buf);
      if (!new_line || *(buf->break_ptr - 1) == '\n') is_str = 1;
    }
    if (!new_line ||
        (buf && (!buf->data || buf->break_ptr == buf->data + buf->amount ||
                 buf->amount <= 0)))
      ft_buf_free(buf, get_buffer_list());
  }
  return (new_line);
}

void ft_gnl_free_fd(int fd) {
  t_buffer *buff = ft_find_fd(get_buffer_list(), fd);
  ft_buf_free(buff, get_buffer_list());
}
