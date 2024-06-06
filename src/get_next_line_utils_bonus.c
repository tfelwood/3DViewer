/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfelwood <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/05 15:11:17 by tfelwood          #+#    #+#             */
/*   Updated: 2021/12/05 15:11:20 by tfelwood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static unsigned long long ft_strlen(char const *s) {
  unsigned long long length;

  length = 0;
  if (s) {
    while (*s++) ++length;
  }
  return (length);
}

char *ft_strjoin(char *s1, const t_buffer *buf) {
  char *new_str;
  char *tmp;
  unsigned long long size;

  new_str = NULL;
  if (buf && buf->data && buf->cur_ptr < buf->break_ptr) {
    size = ft_strlen(s1) + (buf->break_ptr - buf->cur_ptr) + 1;
    new_str = (char *)malloc(size * sizeof(char));
    if (new_str) {
      tmp = s1;
      while (tmp && *tmp) *new_str++ = *tmp++;
      tmp = buf->cur_ptr;
      while (tmp != buf->break_ptr) *new_str++ = *tmp++;
      *new_str = 0;
      new_str -= size - 1;
    }
  }
  free(s1);
  return (new_str);
}

char *ft_find_eofline(t_buffer *buf) {
  char *tmp;

  if (buf && buf->data) {
    tmp = buf->cur_ptr;
    while (tmp < buf->data + buf->amount) {
      if (*tmp == '\n') return (buf->break_ptr = tmp + 1);
      ++tmp;
    }
    return (buf->break_ptr = buf->data + buf->amount);
  }
  return (NULL);
}

char *ft_buf_update(t_buffer *buf) {
  if (!buf->data || buf->break_ptr == buf->data + buf->amount) {
    if (!buf->data) buf->data = (char *)malloc(BUFFER_SIZE * sizeof(char));
    if (buf->data) {
      buf->amount = READ_FUNCTION(buf->fd, buf->data, BUFFER_SIZE);
      if (buf->amount <= 0) return (NULL);
      buf->cur_ptr = buf->data;
      buf->break_ptr = ft_find_eofline(buf);
    }
  } else {
    buf->cur_ptr = buf->break_ptr;
    buf->break_ptr = ft_find_eofline(buf);
  }
  return (buf->data);
}

t_buffer *ft_create_buf(t_buffer *father, int fd) {
  t_buffer *elem;

  elem = (t_buffer *)malloc(sizeof(t_buffer));
  if (elem) {
    elem->next = NULL;
    elem->data = NULL;
    elem->fd = fd;
    elem->prev = father;
  }
  return (elem);
}
