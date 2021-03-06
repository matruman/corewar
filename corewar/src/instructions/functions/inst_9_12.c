/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inst_9_12.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matruman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 17:41:22 by matruman          #+#    #+#             */
/*   Updated: 2020/01/13 15:59:33 by bpole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void			zjmp(t_main *main, t_cursor *cursor, char *area)
{
	int16_t	addr;

	(void)main;
	memory_read(area, cursor->pos + 1, &addr, 2);
	rev_endian(&addr, 2);
	if (cursor->carry)
	{
		cursor->pos += addr % IDX_MOD;
		cursor->pos %= MEM_SIZE;
		if (cursor->pos < 0)
			cursor->pos += MEM_SIZE;
	}
	else
		cursor->pos = (cursor->pos + 3) % MEM_SIZE;
}

void			ldi(t_main *main, t_cursor *cursor, char *area)
{
	char	mem[21];

	(void)main;
	ft_bzero(mem, 20);
	memory_read(area, cursor->pos + 2, mem, 9);
	if (cursor->types[0] == T_REG_CODE && (mem[20] = 1))
	{
		if (mem[0] > 16 || mem[0] < 1)
			return ;
		((int *)(mem + 16))[0] = cursor->reg[mem[0] - 1];
		rev_endian(mem + 16, 4);
	}
	if (cursor->types[0] == T_DIR_CODE && (mem[20] = 2))
	{
		memory_rread(mem, 0, mem + 16, 2);
		((int *)(mem + 16))[0] = *(short *)(mem + 16);
	}
	if (cursor->types[0] == T_IND_CODE && (mem[20] = 2))
	{
		memory_rread(mem, 0, mem + 16, IND_SIZE);
		memory_rread(area, cursor->pos +
		*(short *)(mem + 16) % IDX_MOD, mem + 16, 4);
	}
	ldi_lldi(cursor, area, mem, 0);
}

void			lldi(t_main *main, t_cursor *cursor, char *area)
{
	char	mem[21];

	(void)main;
	ft_bzero(mem, 20);
	memory_read(area, cursor->pos + 2, mem, 9);
	if (cursor->types[0] == T_REG_CODE && (mem[20] = 1))
	{
		if (mem[0] > 16 || mem[0] < 1)
			return ;
		((int *)(mem + 16))[0] = cursor->reg[mem[0] - 1];
		rev_endian(mem + 16, 4);
	}
	if (cursor->types[0] == T_DIR_CODE && (mem[20] = 2))
	{
		memory_rread(mem, 0, mem + 16, 2);
		((int *)(mem + 16))[0] = *(short *)(mem + 16);
	}
	if (cursor->types[0] == T_IND_CODE && (mem[20] = 2))
	{
		memory_rread(mem, 0, mem + 16, IND_SIZE);
		memory_rread(area, cursor->pos +
		*(short *)(mem + 16) % IDX_MOD, mem + 16, 4);
	}
	mem[0] = ldi_lldi(cursor, area, mem, 1);
	cursor->carry = mem[0] < 0 ? cursor->carry : mem[0];
}

static void		t_sti(t_main *main, t_cursor *cursor, char *mem)
{
	int addr;

	if (mem[0] > 16 || mem[0] < 1)
		return ;
	if (cursor->types[2] == T_REG_CODE)
	{
		if (mem[(int)mem[20]] > 16 || mem[(int)mem[20]] < 1)
			return ;
		((int *)(mem + 12))[0] = cursor->reg[mem[(int)mem[20]] - 1];
		rev_endian(mem + 12, 4);
	}
	else if (cursor->types[2] == T_DIR_CODE)
	{
		memory_rread(mem, mem[20], mem + 12, 2);
		((int *)(mem + 12))[0] = *(short *)(mem + 12);
	}
	else
		return ;
	addr = (*(int *)(mem + 12) + *(int *)(mem + 16)) % IDX_MOD;
	memory_write(main, main->cell[cursor->pos].player,
	cursor->pos + addr, &cursor->reg[mem[0] - 1]);
}

void			sti(t_main *main, t_cursor *cursor, char *area)
{
	char	mem[21];

	ft_bzero(mem, 21);
	memory_read(area, cursor->pos + 2, mem, 9);
	if (cursor->types[1] == T_REG_CODE && (mem[20] = 1 + 1))
	{
		if (mem[1] > 16 || mem[1] < 1)
			return ;
		((int *)(mem + 16))[0] = cursor->reg[mem[1] - 1];
		rev_endian(mem + 16, 4);
	}
	if (cursor->types[1] == T_DIR_CODE && (mem[20] = 2 + 1))
	{
		memory_read(mem, 1, mem + 16, 2);
		rev_endian(mem + 16, 2);
		((int *)(mem + 16))[0] = *(short *)(mem + 16);
	}
	if (cursor->types[1] == T_IND_CODE && (mem[20] = 2 + 1))
	{
		memory_rread(mem, 1, mem + 16, IND_SIZE);
		memory_rread(area, cursor->pos +
		*(short *)(mem + 16) % IDX_MOD, mem + 16, 4);
	}
	t_sti(main, cursor, mem);
}
