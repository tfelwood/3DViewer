/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfelwood <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/05 15:11:01 by tfelwood          #+#    #+#             */
/*   Updated: 2023/03/06 14:00:31 by tfelwood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
#define GET_NEXT_LINE_BONUS_H
#include <stdlib.h>
#include <unistd.h>
#define READ_FUNCTION read
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

struct s_buffer;
typedef struct s_buffer t_buffer;
struct s_buffer {
  char *data;
  char *cur_ptr;
  char *break_ptr;
  t_buffer *prev;
  t_buffer *next;
  long long amount;
  int fd;
};

char *ft_find_eofline(t_buffer *buf);
t_buffer *ft_create_buf(t_buffer *father, int fd);
char *ft_buf_update(t_buffer *buf);
char *ft_strjoin(char *s1, const t_buffer *buf);
void ft_gnl_free_fd(int fd);
char *get_next_line(int fd);

#endif
