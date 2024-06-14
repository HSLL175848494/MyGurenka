
FONTSTYLE text_font;
FONTSTYLE ruby_font;
uint font_change;
uint ruby_change;

bool is_text_lib;
int buffer_width;
int buffer_height;
int font_page;
int drop_page;
int color_page;

/////////////////////////////////////////////////////////////////////////////
#define TAG_ERROR		0
#define TAG_OPEN		1
#define TAG_CLOSE		2

typedef struct{
	uchar	state;
	uchar*	name;
	uchar*	attrib[16];
	uchar*	value[16];
	int		count;
} TAG;

TAG tag;
uchar tag_buf[4096];

//===========================================================================
// É^ÉOÇÃéÊìæ
//===========================================================================
uchar* get_tag(uchar* text)
{
	uchar* p = tag_buf;

	tag.state = TAG_ERROR;
	tag.name = NULL;
	tag.count = 0;

	if(*text == '<'){
		text = get_token(text + 1);
		if(token_type == _KEYWORD_){
			tag.name = p;
			strcpy(p, token);
			p += strlen(token) + 1;
			text = get_token(text);

			while(token_type == _KEYWORD_){
				tag.attrib[tag.count] = p;
				strcpy(p, token);
				p += strlen(token) + 1;
				text = get_token(text);

				if(token[0] == '='){
					text = get_token(text);
					if(token_type == _STRING_){
						tag.value[tag.count] = p;
						strcpy(p, token);
						p += strlen(token) + 1;
						text = get_token(text);
						tag.count++;
					}
				}
			}
			tag.state = TAG_OPEN;
		}else if(token[0] == '/'){
			text = get_token(text);
			if(token_type == _KEYWORD_){
				tag.name = p;
				strcpy(p, token);
				p += strlen(token) + 1;
				tag.state = TAG_CLOSE;
			}
		}
		while(token_type != _NULL_ && token[0] != '>')
			text = get_token(text);
	}

	return text;
}
//===========================================================================
// êîílÇÃéÊìæ
//===========================================================================
int get_num(uchar* text, int value)
{
	uchar ch;
	get_token(text);
	if(token_type == _CONST_){
		ch = text[0];
		if(ch == '+' || ch == '-'){
			value += token_value;
		}else{
			value = token_value;
		}
	}
	return value;
}
//===========================================================================
// É^ÉOïtÇ´ÉeÉLÉXÉgÇÃï`âÊ
//===========================================================================
typedef struct{
	uchar*		text;
	short		x;
	short		indent;
	short		font_size;
	short		ruby_size;
} LINEDAT;

extern int check_bracket(int ch)
{
	uchar* bracket_l = "ÅeÅgÅiÅkÅmÅoÅqÅsÅuÅwÅy";
	uchar* bracket_r = "ÅfÅhÅjÅlÅnÅpÅrÅtÅvÅxÅz";
	uchar* pch = mbschr(bracket_l, ch);
	if(pch != NULL){
		pch = &bracket_r[pch - bracket_l];
		return peek_chr(pch);
	}
	return -1;
}

void hyphenation_text(MESSAGE* mes)
{
	static uchar* head_weak = "ÅAÅBÅCÅDÅFÅGÅJÅKÅ]ÅfÅhÅjÅlÅnÅpÅrÅtÅvÅxÅz";
	static uchar* head = "ÅAÅBÅCÅDÅFÅGÅJÅKÅ]ÅfÅhÅjÅlÅnÅpÅrÅtÅvÅxÅz"
						"ÇüÇ°Ç£Ç•ÇßÇ·Ç„ÇÂÇ¡É@ÉBÉDÉFÉHÉbÉÉÉÖÉáÉéÉïÉñÅ[ÅHÅI";
	static uchar* tail = "ÅeÅgÅiÅkÅmÅoÅqÅsÅuÅwÅy";
	static int level = 0;

	static LINEDAT bank[256];
	static int idx;
	static uchar* text;
	static int x;
	static int width;
	static int indent;
	static int bracket;
	static int font_size;
	static int ruby_size;
	static int kinsoku;
	static int count;

	uchar* p;
	uchar* pch;
	int text_width;
	int font_h;
	int ruby_h;
	int align;
	int w, h;
	int hs, vs;
	int i, j, n;
	int ch;

	if(level++ == 0){
		mes->line_count = 0;
		x = mes->x;
		text = mes->text;
		width = mes->width;
		font_size = mes->font_size;
		ruby_size = mes->ruby_size;
		indent = mes->indent;
		bracket = mes->bracket;
		idx = kinsoku = 0;
		count = mes->text_count;
	}

	while(*text != '\0'){
		if(*text == '<'){
			p = text;
			text = get_tag(text);
			if(tag.state == TAG_ERROR){
				continue;
			}
			if(!strcmp(tag.name, "r")){
				if(tag.state == TAG_OPEN){
					font_h = 0;
					ruby_h = mes->ruby_space;
					if(idx > 0){
						for(i = 0; i < idx; i++){
							if(font_h < bank[i].font_size){ font_h = bank[i].font_size; }
							if(ruby_h < bank[i].ruby_size){ ruby_h = bank[i].ruby_size; }
						}
					}else{
						font_h = font_size;
					}
					mes->font_h[mes->line_count] = font_h;
					mes->ruby_h[mes->line_count] = ruby_h;
					mes->next_line[mes->line_count] = text;
					mes->space[mes->line_count] = width - x;
					mes->line_count++;

					x = indent;
					idx = kinsoku = 0;
				}
			}else if(!strcmp(tag.name, "font")){
				if(tag.state == TAG_OPEN){
					n = font_size;
					for(i = 0; i < tag.count; i++){
						if(!strcmp(tag.attrib[i], "size"))
							font_size = get_num(tag.value[i], font_size);
					}
					hyphenation_text(mes);
					font_size = n;
				}else if(tag.state == TAG_CLOSE){
					if(level > 1){
						--level;
						return;
					}
				}
			}else if(!strcmp(tag.name, "ruby")){
				if(tag.state == TAG_OPEN){
					n = ruby_size;
					for(i = 0; i < tag.count; i++){
						if(!strcmp(tag.attrib[i], "size")){
							ruby_size = get_num(tag.value[i], ruby_size);
						}
					}
					hyphenation_text(mes);
					if(idx > 0){
						bank[idx-1].ruby_size = ruby_size;
					}else if(mes->line_count > 0){
						if(mes->ruby_h[mes->line_count-1] < ruby_size){
							mes->ruby_h[mes->line_count-1] = ruby_size;
						}
					}
					ruby_size = n;
				}else if(tag.state == TAG_CLOSE){
					if(level > 1){
						--level;
						return;
					}
				}
			}else if(!strcmp(tag.name, "img")){
				if(tag.state == TAG_OPEN){
					align = 0;
					w = h = mes->font_size;
					hs = vs = 0;
					for(i = 0; i < tag.count; i++){
						if(!strcmp(tag.attrib[i], "align")){
							if(!strcmp(tag.value[i], "left")){
								align = 1;
							}else if(!strcmp(tag.value[i], "right")){
								align = -1;
							}
						}else if(!strcmp(tag.attrib[i], "width")){
							w = get_num(tag.value[i], w);
						}else if(!strcmp(tag.attrib[i], "height")){
							h = get_num(tag.value[i], h);
						}else if(!strcmp(tag.attrib[i], "hspace")){
							hs = get_num(tag.value[i], 0);
						}else if(!strcmp(tag.attrib[i], "vspace")){
							vs = get_num(tag.value[i], 0);
						}
					}
					w += hs * 2;
					h += hs * 2;
					if(w >= width){ align = 0; }
					if(idx > 0){
						if((align > 0) || (x + w > width)){
							font_h = 0;
							ruby_h = mes->ruby_space;
							for(n = 0; n < idx; n++){
								if(font_h < bank[n].font_size){ font_h = bank[n].font_size; }
								if(ruby_h < bank[n].ruby_size){ ruby_h = bank[n].ruby_size; }
							}
							mes->font_h[mes->line_count] = font_h;
							mes->ruby_h[mes->line_count] = ruby_h;
							mes->next_line[mes->line_count] = p;
							mes->space[mes->line_count] = width - x;
							mes->line_count++;

							x = indent;
							idx = 0;
							kinsoku = 0;
						}
					}
					if(align == 0){
						bank[idx].text = p;
						bank[idx].x = x;
						bank[idx].indent = indent;
						bank[idx].font_size = h;
						bank[idx].ruby_size = 0;
						idx++;
						x += w + mes->column_space;
					}else{
						width -= w;
					}
				}
			}
		}else{
			bank[idx].text = text;
			bank[idx].x = x;
			bank[idx].indent = indent;
			bank[idx].font_size = font_size;
			bank[idx].ruby_size = 0;
			idx++;
			kinsoku++;

			ch = get_chr(&text);
			if(ch > 0x00ff){
				x += font_size;
				if(mes->indent_flag && count == 0){
					if((n = check_bracket(ch)) >= 0){
						bracket = n;
						indent = x;
					}
				}else if(ch == bracket){
					bracket = -1;
					indent = 0;
				}
			}else{
				x += font_size / 2;
			}

			text_width = width;
			if(mes->punctuation_flag){
				if(!mbschr(head_weak, peek_chr(bank[idx-1].text))){
					text_width = width - font_size;
				}
			}

			if(x > text_width){
				i = 1;
				if(mes->kinsoku_flag && kinsoku > 2){
					if(mbschr(tail, peek_chr(bank[idx-2].text))){
						i = mbschr(tail, peek_chr(bank[idx-3].text)) ? 3 : 2;
					}else if(mbschr(head, peek_chr(bank[idx-1].text))){
						i = mbschr(head, peek_chr(bank[idx-2].text)) ? 3 : 2;
					}
				}
				j = idx - i;
				mes->space[mes->line_count] = text_width - bank[j].x;
				x = bank[j].indent + (x - bank[j].x);

				font_h = 0;
				ruby_h = mes->ruby_space;
				if(j > 0){
					for(n = 0; n < j; n++){
						if(font_h < bank[n].font_size){ font_h = bank[n].font_size; }
						if(ruby_h < bank[n].ruby_size){ ruby_h = bank[n].ruby_size; }
					}
					memmove(&bank[0], &bank[j], i * sizeof(bank[0]));
					n = -bank[0].x + bank[0].indent;
					for(n = 0; n < i; n++)
						bank[n].x += n;
				}else{
					font_h = bank[0].font_size;
				}
				idx = kinsoku = i;
				mes->font_h[mes->line_count] = font_h;
				mes->ruby_h[mes->line_count] = ruby_h;
				mes->next_line[mes->line_count] = bank[0].text;
				mes->line_count++;
			}
			x += mes->column_space;
			count++;
		}
	}

	if(--level == 0){
		if(idx > 0){
			font_h = 0;
			ruby_h = mes->ruby_space;
			for(i = 0; i < idx; i++){
				if(font_h < bank[i].font_size){ font_h = bank[i].font_size; }
				if(ruby_h < bank[i].ruby_size){ ruby_h = bank[i].ruby_size; }
			}
			mes->font_h[mes->line_count] = font_h;
			mes->ruby_h[mes->line_count] = ruby_h;
			mes->next_line[mes->line_count] = text;
			mes->space[mes->line_count] = width - x;
			mes->line_count++;
		}
	}
}
//===========================================================================
// ÉtÉHÉìÉgï`âÊ
//===========================================================================
void draw_char(MESSAGE* mes, int x, int y, int ch, bool ruby)
{
	FONTSTYLE font;
	RECT rc0, rc1;
	uchar buf[4];
	uint edge_col;
	uint drop_col;
	int w, h;

	if(mes->cpage < 0){ return; }

	if(ruby){
		if(ruby_change & 0x02){
			gfill(color_page, RC(2,0,2,1), mes->ruby_text_col0);
			gfill(color_page, RC(2,1,2,1), mes->ruby_text_col1);
			ruby_change &= ~0x02;
		}
		if(ruby_change & 0x01){
			if(mes->ruby_type == 0){
				strcpy(ruby_font.name, "ÇlÇr ÉSÉVÉbÉN");
			}else{
				strcpy(ruby_font.name, "ÇlÇr ñæí©");
			}
			ruby_font.size = mes->ruby_size;
			ruby_font.style = mes->ruby_style;
			font_get(1, &font);
			if(memcmp(&font, &ruby_font, sizeof(FONTSTYLE))){
				font_set(1, &ruby_font);
			}
			ruby_change &= ~0x01;
		}
		font_use(1);

		w = h = mes->ruby_size;
		edge_col = mes->ruby_edge_col;
		drop_col = mes->ruby_drop_col;
	}else{
		if(font_change & 0x02){
			gfill(color_page, RC(0,0,2,1), mes->font_text_col0);
			gfill(color_page, RC(0,1,2,1), mes->font_text_col1);
			font_change &= ~0x02;
		}
		if(font_change & 0x01){
			if(mes->font_type == 0){
				strcpy(text_font.name, "ÇlÇr ÉSÉVÉbÉN");
			}else{
				strcpy(text_font.name, "ÇlÇr ñæí©");
			}
			text_font.size = mes->font_size;
			text_font.style = mes->font_style;
			font_get(0, &font);
			if(memcmp(&font, &text_font, sizeof(FONTSTYLE))){
				font_set(0, &text_font);
			}
			font_change &= ~0x01;
		}
		font_use(0);

		w = h = mes->font_size;
		if(mes->font_style & FONT_ITALIC){
			w += (w + 3) / 4 + 1;
		}

		edge_col = mes->font_edge_col;
		drop_col = mes->font_drop_col;
	}

	if(ch > 0xff){
		buf[0] = ch >> 8;
		buf[1] = ch;
		buf[2] = '\0';
	}else{
		buf[0] = ch;
		buf[1] = '\0';
		w = (w + 1) / 2;
	}

	rc_box(&rc0, 0, 0, w, h);
	rc_box(&rc1, 0, h+2, w, h);

	gfill(drop_page, RC(0,0,w+2,h*2+4), 0);
	gtext(drop_page, 0, 0, 255, buf);
	gcopy(drop_page, rc1.left, rc1.top+1, drop_page, &rc0, ADD(255));
	gcopy(drop_page, rc1.left, rc1.top-1, drop_page, &rc0, ADD(255));
	gcopy(drop_page, rc1.left-1, rc1.top, drop_page, &rc0, ADD(255));
	gcopy(drop_page, rc1.left+1, rc1.top, drop_page, &rc0, ADD(255));

	if(mes->apage < 0){
		if(mes->drop){
			gcopy(mes->cpage, x+mes->drop_x, y+mes->drop_y, drop_page, &rc1, COLALPHA(255, drop_col));
		}
	}else{
		if(mes->edge){
			gcopy(mes->apage, x, y, drop_page, &rc1, BRIGHT(255));
		}else{
			gcopy(mes->apage, x, y, drop_page, &rc0, BRIGHT(255));
		}
	}

	smooth(font_page, &rc0, color_page, RC(0,0,2,2), -1);
	if(mes->edge){
		gfill(font_page, &rc1, edge_col);
		gcopy(font_page, rc1.left, rc1.top, font_page, &rc0, ALPHA(255));
		gcopy(mes->cpage, x, y, font_page, &rc1, ALPHA(255));
	}else{
		gcopy(mes->cpage, x, y, font_page, &rc0, ALPHA(255));
	}
}
//===========================================================================
// rubyÉ^ÉO
//===========================================================================
void process_ruby(MESSAGE* mes)
{
	RECT rc;
	uchar text[256];
	uchar* p;
	int ch;
	int x, y;
	int n, i;

	int ruby_type = mes->ruby_type;
	int ruby_size = mes->ruby_size;
	uint ruby_text_col0 = mes->ruby_text_col0;
	uint ruby_text_col1 = mes->ruby_text_col1;
	uint ruby_edge_col = mes->ruby_edge_col;
	uint ruby_drop_col = mes->ruby_drop_col;
	uint change = 0;

	text[0] = '\0';

	for(i = 0; i < tag.count; i++){
		if(!strcmp(tag.attrib[i], "text")){
			strcpy(text, tag.value[i]);
		}else if(!strcmp(tag.attrib[i], "type")){
			if(!strcmp(tag.value[i], "ÉSÉVÉbÉN")){ mes->ruby_type = 0; }
			else if(!strcmp(tag.value[i], "ñæí©")){ mes->ruby_type = 1; }
			change |= 0x01;
		}else if(!strcmp(tag.attrib[i], "size")){
			mes->ruby_size = get_num(tag.value[i], mes->ruby_size);
			change |= 0x01;
		}else if(!strcmp(tag.attrib[i], "color")){
			mes->ruby_text_col0 = mes->ruby_text_col1 = get_num(tag.value[i], 0);
			change |= 0x02;
		}else if(!strcmp(tag.attrib[i], "color0")){
			mes->ruby_text_col0 = get_num(tag.value[i], 0);
			change |= 0x02;
		}else if(!strcmp(tag.attrib[i], "color1")){
			mes->ruby_text_col1 = get_num(tag.value[i], 0);
			change |= 0x02;
		}else if(!strcmp(tag.attrib[i], "edge")){
			mes->ruby_edge_col = get_num(tag.value[i], 0);
		}else if(!strcmp(tag.attrib[i], "shadow")){
			mes->ruby_drop_col = get_num(tag.value[i], 0);
		}
	}

	x = mes->x;
	ruby_change |= change;
	process_text(mes);

	if(mes->x < x){ x = 0; }
	n = strlen(text) * mes->ruby_size / 2;
	x += ((mes->x - x) - n) / 2;
	if(mes->align == 1){
		x += mes->space[mes->line_count] / 2;
	}else if(mes->align == 2){
		x += mes->space[mes->line_count];
	}

	if((x + n) > mes->width){ x = mes->width - n; }
	if(x < 0){ x = 0; }
	x += mes->margin_left;
	y = mes->margin_top + mes->y;

	rc_box(&rc, x, y, n, mes->ruby_size);
	if(rc_intersect(&rc, &rc, &mes->rc)){
		text_info[text_info_count].ch = 0;
		rc_copy(&text_info[text_info_count].rc, &rc);
		text_info[text_info_count].drop_color = mes->ruby_drop_col;
		text_info[text_info_count].flag = mes->flag;
		text_info[text_info_count].id = mes->id;
		text_info[text_info_count].line = mes->line_count;
		text_info_count++;
	}

	p = text;
	while(*p != '\0'){
		ch = get_chr(&p);
		draw_char(mes, x, y, ch, true);
		x += (ch > 0xff) ? mes->ruby_size : mes->ruby_size / 2;
	}

	ruby_change |= change;
	mes->ruby_type = ruby_type;
	mes->ruby_size = ruby_size;
	mes->ruby_text_col0 = ruby_text_col0;
	mes->ruby_text_col1 = ruby_text_col1;
	mes->ruby_edge_col = ruby_edge_col;
	mes->ruby_drop_col = ruby_drop_col;
}
//===========================================================================
// fontÉ^ÉO
//===========================================================================
void process_font(MESSAGE* mes)
{
	int font_type = mes->font_type;
	int font_size = mes->font_size;
	uint font_text_col0 = mes->font_text_col0;
	uint font_text_col1 = mes->font_text_col1;
	uint font_edge_col = mes->font_edge_col;
	uint font_drop_col = mes->font_drop_col;
	uint change = 0;
	int ch, i;

	for(i = 0; i < tag.count; i++){
		if(!strcmp(tag.attrib[i], "type")){
			if(!strcmp(tag.value[i], "ÉSÉVÉbÉN")){ mes->font_type = 0; }
			else if(!strcmp(tag.value[i], "ñæí©")){ mes->font_type = 1; }
			change |= 0x01;
		}else if(!strcmp(tag.attrib[i], "size")){
			mes->font_size = get_num(tag.value[i], mes->font_size);
			change |= 0x01;
		}else if(!strcmp(tag.attrib[i], "color")){
			mes->font_text_col0 = mes->font_text_col1 = get_num(tag.value[i], 0);
			change |= 0x02;
		}else if(!strcmp(tag.attrib[i], "color0")){
			mes->font_text_col0 = get_num(tag.value[i], 0);
			change |= 0x02;
		}else if(!strcmp(tag.attrib[i], "color1")){
			mes->font_text_col1 = get_num(tag.value[i], 0);
			change |= 0x02;
		}else if(!strcmp(tag.attrib[i], "edge")){
			mes->font_edge_col = get_num(tag.value[i], 0);
		}else if(!strcmp(tag.attrib[i], "shadow")){
			mes->font_drop_col = get_num(tag.value[i], 0);
		}else if(!strcmp(tag.attrib[i], "align")){
			if(!strcmp(tag.value[i], "left")){
				mes->align = 0;
			}else if(!strcmp(tag.value[i], "center")){
				mes->align = 1;
			}else if(!strcmp(tag.value[i], "right")){
				mes->align = 2;
			}
		}
	}

	font_change |= change;
	process_text(mes);

	font_change |= change;
	mes->font_type = font_type;
	mes->font_size = font_size;
	mes->font_text_col0 = font_text_col0;
	mes->font_text_col1 = font_text_col1;
	mes->font_edge_col = font_edge_col;
	mes->font_drop_col = font_drop_col;
}
//===========================================================================
// imgÉ^ÉO
//===========================================================================
void process_img(MESSAGE* mes)
{
	uchar* file[256];
	GINFO gi;
	RECT rc0, rc1;
	int align;
	int page;
	int x, y;
	int w, h;
	int hs, vs;
	int i;

	align = 0;
	w = h = mes->font_size;
	hs = vs = 0;
	for(i = 0; i < tag.count; i++){
		if(!strcmp(tag.attrib[i], "src")){
			sprintf(file, "gfx\\%s.bmp", tag.value[i]);
		}else if(!strcmp(tag.attrib[i], "align")){
			if(!strcmp(tag.value[i], "left")){
				align = 1;
			}else if(!strcmp(tag.value[i], "right")){
				align = -1;
			}
		}else if(!strcmp(tag.attrib[i], "width")){
			w = get_num(tag.value[i], w);
		}else if(!strcmp(tag.attrib[i], "height")){
			h = get_num(tag.value[i], h);
		}else if(!strcmp(tag.attrib[i], "hspace")){
			hs = get_num(tag.value[i], 0);
		}else if(!strcmp(tag.attrib[i], "vspace")){
			vs = get_num(tag.value[i], 0);
		}
	}
	if((w + hs * 2) >= mes->width){
		align = 0;
	}

	if(align == 0){
		x = mes->margin_left + mes->x + hs;
		if(mes->align == 1){
			x += mes->space[mes->line_count] / 2;
		}else if(mes->align == 2){
			x += mes->space[mes->line_count];
		}
		y = mes->margin_top + mes->y + mes->ruby_h[mes->line_count] + mes->font_h[mes->line_count] - h - vs;
	}else if(align > 0){
		x = mes->margin_left + hs;
		y = mes->margin_top + mes->y + vs;
		mes->margin_left += w + hs * 2;
		mes->width -= w + hs * 2;
	}else if(align < 0){
		x = mes->margin_left + mes->width - w - hs;
		y = mes->margin_top + mes->y + vs;
		mes->margin_right += w + hs * 2;
		mes->width -= w + hs * 2;
	}

	if((mes->cpage >= 0) && gload(&page, file)){
		resize_text_buffer(w, h);
		ginfo(page, &gi);

		rc_box(&rc0, 0, 0, w, h);
		rc_box(&rc1, 0, h+2, w, h);

		if(gi.depth == 8){
			if(font_change & 0x02){
				gfill(color_page, RC(0,0,2,1), mes->font_text_col0);
				gfill(color_page, RC(0,1,2,1), mes->font_text_col1);
				font_change &= ~0x02;
			}
//			gblit(drop_page, &rc0, page, NULL, NULL, 0);
			smooth(drop_page, &rc0, page, NULL, -1);
			smooth(font_page, &rc0, color_page, RC(0,0,2,2), -1);
		}else if(gi.depth == 32){
//			gblit(font_page, &rc0, page, NULL, ALPHA(255), 0);
			gblit(drop_page, &rc0, page, NULL, ACHANNEL(), 0);
			smooth(font_page, &rc0, page, NULL, -1);
		}else{
			gfill(drop_page, &rc0, PAL(255));
			smooth(font_page, &rc0, page, NULL, -1);
		}

		gradation(drop_page, rc1.left, rc1.top, drop_page, &rc0);
		gcopy(drop_page, rc1.left, rc1.top, drop_page, &rc0, ADD(255));

		if(mes->apage < 0){
			if(mes->drop){
				gcopy(mes->cpage, x+mes->drop_x, y+mes->drop_y, drop_page, &rc1, COLALPHA(255, mes->font_drop_col));
			}
		}else{
			if(mes->edge){
				gcopy(mes->apage, x, y, drop_page, &rc1, BRIGHT(255));
			}else{
				gcopy(mes->apage, x, y, drop_page, &rc0, BRIGHT(255));
			}
		}

		if(mes->edge){
			gfill(font_page, &rc1, mes->font_edge_col);
			gcopy(font_page, rc1.left, rc1.top, font_page, &rc0, ALPHA(255));
			gcopy(mes->cpage, x, y, font_page, &rc1, ALPHA(255));
		}else{
			gcopy(mes->cpage, x, y, font_page, &rc0, ALPHA(255));
		}

		gfree(page);
	}

	rc_box(&rc0, x, y, w, h);
	if(rc_intersect(&rc0, &rc0, &mes->rc)){
		text_info[text_info_count].ch = -1;
		rc_copy(&text_info[text_info_count].rc, &rc0);
		text_info[text_info_count].drop_color = mes->font_drop_col;
		text_info[text_info_count].flag = mes->flag;
		text_info[text_info_count].id = mes->id;
		text_info[text_info_count].line = mes->line_count;
		text_info_count++;
	}

	if(align == 0){
		mes->x += w + mes->column_space;
	}
}
//===========================================================================
// emÉ^ÉO
//===========================================================================
void process_em(MESSAGE* mes)
{
	int flag = mes->flag;
	int i;

	mes->flag = 1;
	for(i = 0; i < tag.count; i++){
		if(!strcmp(tag.attrib[i], "type")){
			mes->flag = get_num(tag.value[i], mes->flag);
		}
	}
	process_text(mes);
	mes->flag = flag;
}
//===========================================================================
// keyÉ^ÉO
//===========================================================================
void process_key(MESSAGE* mes)
{
	int id;
	int i;

	if(conv_key_to_id){
		id = mes->id;
		mes->id = 0;
		for(i = 0; i < tag.count; i++){
			if(!strcmp(tag.attrib[i], "id")){
				mes->id = (*conv_key_to_id)(tag.value[i]);
			}
		}
		process_text(mes);
		mes->id = id;
	}
}
//===========================================================================
// ÇPï∂éöï`âÊ
//===========================================================================
void process_char(MESSAGE* mes)
{
	RECT rc;
	uchar* pch;
	int ch;
	int x, y;
	int n, w;

	ch = get_chr(&mes->text);
	if(ch > 0x00ff){
		w = mes->font_size;
		if(mes->indent_flag && mes->text_count == 0){
			if((n = check_bracket(ch)) >= 0){
				mes->bracket = n;
				mes->indent = mes->x + w + mes->column_space;
			}
		}else if(ch == mes->bracket){
			mes->bracket = -1;
			mes->indent = 0;
		}
	}else{
		w = mes->font_size / 2;
	}

//	if(ch != 'Å@' && ch != ' '){
		x = mes->margin_left + mes->x;
		if(mes->align == 1){
			x += mes->space[mes->line_count] / 2;
		}else if(mes->align == 2){
			x += mes->space[mes->line_count];
		}
		y = mes->margin_top + mes->y + mes->ruby_h[mes->line_count] + mes->font_h[mes->line_count] - mes->font_size;
		draw_char(mes, x, y, ch, false);

		rc_box(&rc, x, y, w, mes->font_size);
		if(rc_intersect(&rc, &rc, &mes->rc)){
			text_info[text_info_count].ch = ch;
			rc_copy(&text_info[text_info_count].rc, &rc);
			text_info[text_info_count].drop_color = mes->font_drop_col;
			text_info[text_info_count].flag = mes->flag;
			text_info[text_info_count].id = mes->id;
			text_info[text_info_count].line = mes->line_count;
			text_info_count++;
		}
		mes->text_count++;
//	}

	mes->x += w + mes->column_space;
}
//===========================================================================
// ï∂éöóÒâêÕ
//===========================================================================
void process_text(MESSAGE* mes)
{
	static int level = 0;
	static FONTSTYLE font;
	static bool flag;

	if(level++ == 0){
//		mes->bracket = -1;
//		mes->indent = 0;
//		mes->text_count = 0;
		mes->line_count = 0;
		font_change = 0x03;
		ruby_change = 0x03;
		text_info_count = 0;
		flag = false;
	}

	while(*mes->text != '\0'){
		if(mes->text == mes->next_line[mes->line_count]){
			mes->x = mes->indent;
			mes->y += mes->ruby_h[mes->line_count] + mes->font_h[mes->line_count] + mes->line_space;
			mes->line_count++;
			flag = false;
		}
		if(mes->y >= mes->height){
			break;
		}
		if(*mes->text == '<'){
			mes->text = get_tag(mes->text);
			if(tag.state == TAG_ERROR){
				continue;
			}
			if(!strcmp(tag.name, "font")){
				if(tag.state == TAG_OPEN){
					process_font(mes);
				}else if(tag.state == TAG_CLOSE){
					if(level > 1){
						--level;
						return;
					}
				}
			}else if(!strcmp(tag.name, "ruby")){
				if(tag.state == TAG_OPEN){
					process_ruby(mes);
				}else if(tag.state == TAG_CLOSE){
					if(level > 1){
						--level;
						return;
					}
				}
			}else if(!strcmp(tag.name, "img")){
				if(tag.state == TAG_OPEN){
					process_img(mes);
				}
			}else if(!strcmp(tag.name, "b")){
				if(tag.state == TAG_OPEN){
					if(++mes->bold == 1){
						mes->font_style |= FONT_BOLD;
						font_change |= 0x01;
					}
				}else if(tag.state == TAG_CLOSE){
					if(--mes->bold <= 0){
						mes->bold = 0;
						mes->font_style &= ~FONT_BOLD;
						font_change |= 0x01;
					}
				}
			}else if(!strcmp(tag.name, "i")){
				if(tag.state == TAG_OPEN){
					if(++mes->italic == 1){
						mes->font_style |= FONT_ITALIC;
						font_change |= 0x01;
					}
				}else if(tag.state == TAG_CLOSE){
					if(--mes->italic <= 0){
						mes->italic = 0;
						mes->font_style &= ~FONT_ITALIC;
						font_change |= 0x01;
					}
				}
			}else if(!strcmp(tag.name, "u")){
				if(tag.state == TAG_OPEN){
					if(++mes->underline == 1){
						mes->font_style |= FONT_UNDERLINE;
						font_change |= 0x01;
					}
				}else if(tag.state == TAG_CLOSE){
					if(--mes->underline <= 0){
						mes->underline = 0;
						mes->font_style &= ~FONT_UNDERLINE;
						font_change |= 0x01;
					}
				}
			}else if(!strcmp(tag.name, "s")){
				if(tag.state == TAG_OPEN){
					if(++mes->strikeout == 1){
						mes->font_style |= FONT_STRIKEOUT;
						font_change |= 0x01;
					}
				}else if(tag.state == TAG_CLOSE){
					if(--mes->strikeout <= 0){
						mes->strikeout = 0;
						mes->font_style &= ~FONT_STRIKEOUT;
						font_change |= 0x01;
					}
				}
			}else if(!strcmp(tag.name, "em")){
				if(tag.state == TAG_OPEN){
					process_em(mes);
				}else if(tag.state == TAG_CLOSE){
					if(level > 1){
						--level;
						return;
					}
				}
			}else if(!strcmp(tag.name, "key")){
				if(tag.state == TAG_OPEN){
					process_key(mes);
				}else if(tag.state == TAG_CLOSE){
					if(level > 1){
						--level;
						return;
					}
				}
			}else if(!strcmp(tag.name, "r")){
				flag = true;
			}
		}else{
			process_char(mes);
		}
	}

	if(--level == 0){
		if(flag){
			mes->x = mes->indent;
			mes->y += mes->ruby_h[mes->line_count] + mes->font_h[mes->line_count] + mes->line_space;
			mes->line_count++;
		}
	}
}
//===========================================================================
// ÉÇÉWÉÖÅ[Éãèâä˙âª
//===========================================================================
extern void init_text_lib()
{
	free_text_lib();

	if(!is_text_lib){
		buffer_width = 80;
		buffer_height = (buffer_width + 2) * 2;
		conv_key_to_id = NULL;
		galloc(&font_page, buffer_width, buffer_height, 24);
		galloc(&drop_page, buffer_width, buffer_height, 8);
		gattach(font_page, drop_page);
		galloc(&color_page, 4, 2, 24);
		gfill(color_page, NULL, 0xffffff);
		is_text_lib = true;
	}
}
//===========================================================================
// ÉÇÉWÉÖÅ[Éãâï˙
//===========================================================================
extern void free_text_lib()
{
	if(is_text_lib){
		gfree(font_page);
		gfree(drop_page);
		gfree(color_page);
		buffer_width = 0;
		buffer_height = 0;
		conv_key_to_id = NULL;
		is_text_lib = false;
	}
}
//===========================================================================
// ÉtÉHÉìÉgÉoÉbÉtÉ@ÇÃÉäÉTÉCÉY
//===========================================================================
extern void resize_text_buffer(int width, int height)
{
	bool resize = false;

	width = width + 2;
	height = (height + 2) * 2;

	if(is_text_lib){
		if(width > buffer_width){
			buffer_width = width;
			resize = true;
		}
		if(height > buffer_height){
			buffer_height = height;
			resize = true;
		}
		if(resize){
			galloc(font_page, buffer_width, buffer_height, 24);
			galloc(drop_page, buffer_width, buffer_height, 8);
			gattach(font_page, drop_page);
		}
	}
}
//===========================================================================
// ï∂éöóÒÇÃï`âÊ
//===========================================================================
extern int print(MESSAGE* pmes)
{
	MESSAGE mes;
	GINFO gi;

	text_info_count = 0;

	if(is_text_lib){
		memcpy(&mes, pmes, sizeof(MESSAGE));

		if(mes.cpage >= 0){
			ginfo(mes.cpage, &gi);
			rc_box(&mes.rc, 0, 0, gi.width, gi.height);
			mes.width = gi.width - (mes.margin_left + mes.margin_right);
			mes.height = gi.height - (mes.margin_top + mes.margin_bottom);
			mes.depth = gi.depth;
		}else{
			if(rc_isempty(&mes.rc)){
				rc_box(&mes.rc, 0, 0, mes.width, mes.height);
			}else{
				mes.width = mes.rc.right - (mes.margin_left + mes.margin_right);
				mes.height = mes.rc.bottom - (mes.margin_top + mes.margin_bottom);
			}
		}
		if(mes.width > 0 && mes.height > 0){
			hyphenation_text(&mes);
			process_text(&mes);
		}

		if(mes.cpage >= 0){
			pmes->x = mes.x;
			pmes->y = mes.y;
			pmes->indent = mes.indent;
			pmes->bracket = mes.bracket;
			pmes->text_count = mes.text_count;
		}
	}

	return text_info_count;
}
//===========================================================================
// ÉtÉHÉìÉgÇÃèâä˙âª
//===========================================================================
extern void init_font(int size, uint col, uint edge, uint drop)
{
	memset(&mess, 0, sizeof(mess));
	mess.apage = -1;
	mess.font_size = size;
	mess.ruby_size = size / 2;
	mess.font_style = FONT_ANTIALIAS;
	mess.ruby_style = FONT_ANTIALIAS | FONT_BOLD;
	mess.font_text_col0 = mess.ruby_text_col0 = col;
	mess.font_text_col1 = mess.ruby_text_col1 = col;
	mess.drop_x = mess.drop_y = 2;
	if(edge & 0x80000000){
		mess.edge = false;
	}else{
		mess.edge = true;
		mess.font_edge_col = mess.ruby_edge_col = edge;
	}
	if(drop & 0x80000000){
		mess.drop = false;
	}else{
		mess.drop = true;
		mess.font_drop_col = mess.ruby_drop_col = drop;
	}
	mess.bracket = -1;
}
//===========================================================================
// ï∂éöóÒÇÃï`âÊ
//===========================================================================
extern void draw_text(int page, int x, int y, uchar* text)
{
	mess.cpage = page;
	mess.text = text;
	mess.margin_left = x;
	mess.margin_top = y;
	print(&mess);
}
//===========================================================================
// ï`âÊîÕàÕÇÃéÊìæ
//===========================================================================
extern void get_text_rect(RECT* prc)
{
	int i;

	if(prc != NULL){
		rc_empty(prc);
		for(i = 0; i < text_info_count; i++){
			rc_union(prc, prc, &text_info[i].rc);
		}
	}
}
