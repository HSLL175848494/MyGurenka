//===========================================================================
// 僄儔乕昞帵
//===========================================================================
extern void error(uchar* err)
{
	mbox(MB_OK, "error!", err);
	exit();
}
//===========================================================================
// 僼僅儖僟柤偺庢摼
//===========================================================================
extern void get_dir_name(uchar* path, uchar* dir)
{
	uchar buf[256];
	uchar* last;
	uchar* p;

	if(!path || !dir){ return; }

	mbscpy(buf, path);
	last = NULL;
	p = mbschr(buf, '\\');
	while(p != NULL){
		last = p;
		p = mbschr(p+1, '\\');
	}
	if(last != NULL){
		*(last+1) = '\0';
		mbscpy(dir, buf);
	}else{
		*dir = '\0';
	}
}
//===========================================================================
// 僼傽僀儖偺庢摼
//===========================================================================
extern void get_file_name(uchar* path, uchar* file)
{
	uchar buf[256];
	uchar* last;
	uchar* p;

	if(!path || !file){ return; }

	mbscpy(buf, path);
	last = NULL;
	p = mbschr(buf, '\\');
	while(p != NULL){
		last = p;
		p = mbschr(p+1, '\\');
	}
	if(last != NULL){
		mbscpy(file, last+1);
	}else{
		mbscpy(file, path);
	}
}
//===========================================================================
// 僨乕僞僼傽僀儖偺撉傒崬傒
//===========================================================================
extern bool load_data(uchar* file, int size, uchar** data, short* count)
{
	HFILE fp;
	uint id;
	uchar buf[256];
	int read;

	*data = NULL;
	*count = 0;

	if(!(fp = pack_open(file))){
		sprintf(buf, "%s : 僨乕僞僼傽僀儖偑僆乕僾儞偱偒傑偣傫", file);
		error(buf);
		return false;
	}

	pack_read(fp, &id, sizeof(id));
	if(id != 0x01234567){
		pack_close(fp);
		sprintf(buf, "%s : 僼傽僀儖宍幃偑堘偄傑偡", file);
		error(buf);
		return false;
	}

	pack_read(fp, count, sizeof(short));

	*data = malloc(*count * size);
	if(*data == NULL){
		pack_close(fp);
		sprintf(buf, "%s : 儊儌儕偺妋曐偵幐攕偟傑偟偨", file);
		error(buf);
		return false;
	}

	read = pack_read(fp, *data, *count * size);
	if(read != (*count * size)){
		pack_close(fp);
		sprintf(buf, "%s : 僼傽僀儖偺撉傒崬傒偵幐攕偟傑偟偨", file);
		error(buf);
		return false;
	}

	pack_close(fp);

	return true;
}

extern int load_file(char* file, uchar** data)
{
	HFILE fp;
	uchar buf[256];
	int size;
	int read;

	*data = NULL;
	if(!(fp = pack_open(file))){
		sprintf(buf, "%s : 僼傽僀儖偺僆乕僾儞偵幐攕偟傑偟偨", file);
		error(buf);
		return 0;
	}

	size = pack_size(fp);
	*data = malloc(size);
	if(*data == NULL){
		pack_close(fp);
		sprintf(buf, "%s : 儊儌儕偺妋曐偵幐攕偟傑偟偨", file);
		error(buf);
		return 0;
	}

	read = pack_read(fp, *data, size);
	if(read != size){
		free(*data);
		*data = NULL;
		pack_close(fp);
		sprintf(buf, "%s : 僼傽僀儖偺撉傒崬傒偵幐攕偟傑偟偨", file);
		error(buf);
		return 0;
	}

	pack_close(fp);

	return size;
}
//===========================================================================
// 僨乕僞儕僗僩
//===========================================================================
extern bool list_data(DATALIST* list, uint id, int elem_size, uchar** data, int* count)
{
	DATALIST* curr;
	DATALIST* last;
	uchar buf[256];

	if((list == NULL) || (data == NULL) || (count == NULL)){
		error("list_data: 堷悢僄儔乕");
		return false;
	}
	if(list->id != 0x5453494c){
		error("list_data: 僨乕僞宍幃偑堘偄傑偡");
		return false;
	}
	curr = list + 1;
	last = (uchar*)curr + list->size;
	while(curr < last){
		if(curr->id == id){
			if((curr->size % elem_size) != 0){
				sprintf(buf, "list_data: No.%d偺僨乕僞僒僀僘偑晄堦抳%d", id, curr->size / elem_size);
				error(buf);
				return false;
			}
			*data  = curr + 1;
			*count = curr->size / elem_size;
			return true;
		}
		curr = (uchar*)(curr + 1) + curr->size;
	}
	error("list_data: 僨乕僞偑尒偮偐傝傑偣傫");
	return false;
}
//===========================================================================
// 僉儍僢僔儏夋憸偺弶婜壔
//===========================================================================
extern void init_image_lib()
{
	memset(image, 0, sizeof(IMAGE)*IMAGE_MAX);
	cache_image = true;
}
//===========================================================================
// 僉儍僢僔儏夋憸偺夝曻
//===========================================================================
extern void free_image_lib()
{
	int i;

	for(i = 0; i < IMAGE_MAX; i++){
		if(image[i].ref > 0){
			image[i].ref = 1;
			free_image(&image[i]);
		}
	}
}
//===========================================================================
// 夋憸偺儘乕僪
//===========================================================================
extern IMAGE* load_image(uchar* file)
{
	GINFO gi;
	int i;

	if(cache_image){
		// 夋憸僼傽僀儖偑婛偵搊榐偝傟偰偄傞偐挷傋傞
		for(i = 0; i < IMAGE_MAX; i++){
			if(image[i].ref > 0 && image[i].cache){
				if(!strcmpi(file, image[i].file)){
					image[i].ref++;
					return &image[i];
				}
			}
		}
	}

	// 夋憸僼傽僀儖偺怴婯搊榐
	for(i = 0; i < IMAGE_MAX; i++){
		if(image[i].ref <= 0){
			memset(&image[i], 0, sizeof(IMAGE));

			strcpy(image[i].file, file);
			if(!gload(&image[i].page, file)){
				return NULL;
			}
			image[i].back_page = -1;

			ginfo(image[i].page, &gi);
			image[i].width  = gi.width;
			image[i].height = gi.height;
			image[i].depth  = gi.depth;

			image[i].cache = cache_image;
			image[i].is_file = true;
			image[i].ref = 1;
			return &image[i];
		}
	}

	return NULL;
}
//===========================================================================
// 夋柺僶僢僼傽偺嶌惉
//===========================================================================
extern IMAGE* alloc_image(uchar* file, int width, int height, int depth)
{
	int i;

	// 夋憸僼傽僀儖偑婛偵搊榐偝傟偰偄傞偐挷傋傞
	for(i = 0; i < IMAGE_MAX; i++){
		if(image[i].ref > 0 && image[i].cache){
			if(!strcmpi(file, image[i].file)){
				image[i].ref++;
				return &image[i];
			}
		}
	}

	// 夋憸僼傽僀儖偺怴婯搊榐
	for(i = 0; i < IMAGE_MAX; i++){
		if(image[i].ref <= 0){
			memset(&image[i], 0, sizeof(IMAGE));

			strcpy(image[i].file, file);
			if(!galloc(&image[i].page, width, height, depth)){
				return NULL;
			}
			image[i].back_page = -1;

			image[i].width  = width;
			image[i].height = height;
			image[i].depth  = depth;

			image[i].cache = cache_image;
			image[i].is_file = false;
			image[i].ref = 1;
			return &image[i];
		}
	}

	return NULL;
}
//===========================================================================
// 夋憸偺夝曻
//===========================================================================
extern void free_image(IMAGE* img)
{
	if((img != NULL) && (--img->ref == 0)){
		gfree(img->page);
		gfree(img->back_page);
	}
}
//===========================================================================
// 椞堟妋曐
//===========================================================================
extern bool alloc_data(MTABLE* tbl, uint size, uint *last, uint max)
{
	char buf[256];

	if((*last + size) >= max){
		sprintf(buf, "僨乕僞椞堟偺妋曐偵幐攕偟傑偟偨丅(%d+%d>=%d)", *last, size, max);
		error(buf);
		return false;
	}
	tbl->index = *last;
	tbl->size = size;

	*last += size;

	return true;
}
//===========================================================================
// 椞堟夝曻
//===========================================================================
extern void free_data(uchar* data, MTABLE* tbl, uint size, uint *last, bool move_flg)
{
	int move_index = tbl->index + tbl->size;
	int move_size = *last - move_index;

	if(move_flg && (move_size > 0)){
		memmove(data+tbl->index*size, data+move_index*size, move_size*size);
	}

	memset(data+(tbl->index+move_size)*size, 0, tbl->size*size);

	*last -= tbl->size;
}
//===========================================================================
// 1偵側偭偰偄傞價僢僩偺悢傪庢摼
//===========================================================================
extern int get_bit_count(uint bits)
{
	bits = (bits & 0x55555555) + (bits >> 1 & 0x55555555);
	bits = (bits & 0x33333333) + (bits >> 2 & 0x33333333);
	bits = (bits & 0x0f0f0f0f) + (bits >> 4 & 0x0f0f0f0f);
	bits = (bits & 0x00ff00ff) + (bits >> 8 & 0x00ff00ff);
	return (bits & 0x0000ffff) + (bits >>16 & 0x0000ffff);
}
//===========================================================================
// 巜掕斖埻撪偺棎悢傪惗惉
//===========================================================================
extern int rand_range(int min, int max)
{
	return min + (rand() % (max - min + 1));
}
//===========================================================================
// 抣偺斖埻惂尷
//===========================================================================
extern int limit(int value, int min, int max)
{
	if(value < min){ return min; }
	if(value > max){ return max; }
	return value;
}

extern int min(int a, int b)
{
	return (a < b) ? a : b;
}

extern int max(int a, int b)
{
	return (a > b) ? a : b;
}
//===========================================================================
// 曻暔慄嵗昗庢摼
//===========================================================================
extern void parabora(int *x, int *y, int dx, int dy, int h, float t)
{
	float tmp = 0.5 - t;
	if(x != NULL){ *x = dx * t; }
	if(y != NULL){ *y = dy * t + h * 4 * tmp * tmp - h; }
}
//===========================================================================
// 慄暘岎嵎敾掕
//===========================================================================
extern bool intersect_line(POINT* pa0, POINT* pa1, POINT* pb0, POINT* pb1, POINT* pc)
{
	int a1, b1, c1, a2, b2, c2;

	if(pa0->x >= pa1->x){
		if((pa0->x < pb0->x && pa0->x < pb1->x) ||
			(pa1->x > pb0->x && pa1->x > pb1->x)){
			return false;
		}
	}else{
		if((pa1->x < pb0->x && pa1->x < pb1->x) ||
			(pa0->x > pb0->x && pa0->x > pb1->x)){
			return false;
		}
	}
	if(pa0->y >= pa1->y){
		if((pa0->y < pb0->y && pa0->y < pb1->y) ||
			(pa1->y > pb0->y && pa1->y > pb1->y)){
			return false;
		}
	}else{
		if((pa1->y < pb0->y && pa1->y < pb1->y) ||
			(pa0->y > pb0->y && pa0->y > pb1->y)){
			return false;
		}
	}
	if(((pa0->x - pa1->x) * (pb0->y - pa0->y) +
		(pa0->y - pa1->y) * (pa0->x - pb0->x)) *
		((pa0->x - pa1->x) * (pb1->y - pa0->y) +
		(pa0->y - pa1->y) * (pa0->x - pb1->x)) > 0){
		return false;
	}
	if(((pb0->x - pb1->x) * (pa0->y - pb0->y) +
		(pb0->y - pb1->y) * (pb0->x - pa0->x)) *
		((pb0->x - pb1->x) * (pa1->y - pb0->y) +
		(pb0->y - pb1->y) * (pb0->x - pa1->x)) > 0){
		return false;
	}
	if(pc != NULL){
		a1 = pa0->y - pa1->y;
		b1 = pa1->x - pa0->x;
		c1 = pa0->x * pa1->y - pa1->x * pa0->y;
		a2 = pb0->y - pb1->y;
		b2 = pb1->x - pb0->x;
		c2 = pb0->x * pb1->y - pb1->x * pb0->y;
		pc->x = (b1*c2 - b2*c1) / (a1*b2 - a2*b1);
		pc->y = (a1*c2 - a2*c1) / (a2*b1 - a1*b2);
	}
	return true;
}
//===========================================================================
// 揰偺懡妏宍偵懳偡傞撪奜敾掕
//===========================================================================
extern bool pt_in_polygon(POINT* target, POINT* points, int count)
{
	POINT* p0;
	POINT* p1;
	bool x0, y0;
	bool x1, y1;
	int hit;
	int i;

	hit = 0;
	p0 = &points[0];
	x0 = (target->x <= p0->x);
	y0 = (target->y <= p0->y);

	for(i = 1; i < count+1; i++){
		p1 = &points[i%count];
		x1 = (target->x <= p1->x);
		y1 = (target->y <= p1->y);
		if(y0 != y1){
			if(x0 == x1){
				if(x0){
					hit += (y0 ? -1 : 1);
				}
			}else{
				if(target->x <= (p0->x + (p1->x - p0->x) * (target->y - p0->y) / (p1->y - p0->y))){
					hit += (y0 ? -1 : 1);
				}
			}
		}
		p0 = p1;
		x0 = x1;
		y0 = y1;
	}
	return (hit != 0);
}
//===========================================================================
// 恑峴帪娫偺庢摼
//===========================================================================
float prog_time(uint time, uint min, uint max)
{
	uint interval = max - min;
	return (float)limit(time-min, 0, interval) / interval;
}
//===========================================================================
// 慄宍曗娫
//===========================================================================
extern int flat_speed(int a, int b, float t)
{
	return a + (int)((b - a) * t);
}

extern int fast_speed(int a, int b, float t)
{
	return a + (int)((b - a) * t * t);
}

extern int slow_speed(int a, int b, float t)
{
	return a + (int)((b - a) * sqrt(t));
}
//===========================================================================
// 慄宍曗娫
//===========================================================================
float trans_linear(float p)
{
	return p;
}

float trans_pow(float p)
{
	float x = 2.0;
	return pow(p, x);
}

float trans_expo(float p)
{
	return pow(2.0, 8.0 * (p - 1.0));
}

float trans_circ(float p)
{
	return 1.0 - sin(acos(p));
}

float trans_sine(float p)
{
	return 1.0 - sin((1.0 - p) * PI / 2.0);
}

float trans_back(float p)
{
	float x = 1.618;
	return pow(p, 2.0) * ((x + 1.0) * p - x);
}

float trans_bounce(float p)
{
	float value;
	float a = 0.0;
	float b = 1.0;
	while( true ){
		if(p >= (7.0 - 4.0 * a) / 11.0){
			value = -pow((11.0 - 6.0 * a - 11.0 * p) / 4.0, 2.0) + b * b;
			break;
		}
		a += b;
		b /= 2.0;
	}
	return value;
}

float trans_elastic(float p)
{
	float x = 1.0;
	return pow(2.0, 10.0 * (p - 1.0)) * cos(20.0 * (p - 1.0) * PI * x / 3.0);
}

static float (*trans_func[8])() = {
	trans_linear,
	trans_pow,
	trans_expo,
	trans_circ,
	trans_sine,
	trans_back,
	trans_bounce,
	trans_elastic
};

extern float ftrans_in(int mode, float a, float b, float p)
{
	if(mode >= TR_MAX){ return a; }
	return a + (b - a) * (*trans_func[mode])(p);
}

extern float ftrans_out(int mode, float a, float b, float p)
{
	if(mode >= TR_MAX){ return a; }
	return a + (b - a) * (1.0 - (*trans_func[mode])(1.0 - p));
}

extern float ftrans_inout(int mode, float a, float b, float p)
{
	if(mode >= TR_MAX){ return a; }
	if(p <= 0.5){
		return a + (b - a) * ((*trans_func[mode])(2.0 * p) / 2.0);
	}else{
		return a + (b - a) * ((2.0 - (*trans_func[mode])(2.0 * (1.0 - p))) / 2.0);
	}
}

extern float ftrans(int mode, float a, float b, float p)
{
	int inout = (mode >> 8) & 0xff;
	int trans = mode & 0xff;
	if(inout == TR_IN){
		return ftrans_in(trans, a, b, p);
	}else if(inout == TR_OUT){
		return ftrans_out(trans, a, b, p);
	}else if(inout == TR_INOUT){
		return ftrans_inout(trans, a, b, p);
	}
}

extern int itrans_in(int mode, int a, int b, float p)
{
	if(mode >= TR_MAX){ return a; }
	return a + (int)((b - a) * (*trans_func[mode])(p));
}

extern int itrans_out(int mode, int a, int b, float p)
{
	if(mode >= TR_MAX){ return a; }
	return a + (int)((b - a) * (1.0 - (*trans_func[mode])(1.0 - p)));
}

extern int itrans_inout(int mode, int a, int b, float p)
{
	if(mode >= TR_MAX){ return a; }
	if(p <= 0.5){
		return a + (int)((b - a) * ((*trans_func[mode])(2.0 * p) / 2.0));
	}else{
		return a + (int)((b - a) * ((2.0 - (*trans_func[mode])(2.0 * (1.0 - p))) / 2.0));
	}
}

extern int itrans(int mode, int a, int b, float p)
{
	int inout = (mode >> 8) & 0xff;
	int trans = mode & 0xff;
	if(inout == TR_IN){
		return itrans_in(trans, a, b, p);
	}else if(inout == TR_OUT){
		return itrans_out(trans, a, b, p);
	}else if(inout == TR_INOUT){
		return itrans_inout(trans, a, b, p);
	}
}
//===========================================================================
// 僋僀僢僋僜乕僩
//===========================================================================
extern void qsort(uchar* base, int count, int size, int (*comp)())
{
	uchar* head  = base;
	uchar* tail  = base + (count - 1) * size;
	uchar* pivot = base + (count / 2) * size;

	uchar* left  = head;
	uchar* right = tail;

	for(;;){
		while((*comp)(left,  pivot) < 0) left  += size;
		while((*comp)(right, pivot) > 0) right -= size;
		if(left >= right){ break; }
		memswap(left, right, size);
		if     (pivot == left ){ pivot = right; }
		else if(pivot == right){ pivot = left; }
		left  += size;
		right -= size;
	}
	left  -= size;
	right += size;
	if(head  < left){ qsort(head,  (left - head ) / size + 1, size, comp); }
	if(right < tail){ qsort(right, (tail - right) / size + 1, size, comp); }
}
//===========================================================================
// 儅乕僕僜乕僩
//===========================================================================
uchar* _msort_buf;

void _msort(uchar* left, uchar* right, int size, int (*comp)())
{
	if(left >= right){ return; }

	int last = (right - left) / size;
	uchar* middle = left + (last / 2) * size;

	_msort(left, middle, size, comp);
	_msort(middle+size, right, size, comp);

	uchar* dest = _msort_buf;
	uchar* src = left;
	while(src <= middle){
		memcpy(dest, src, size);
		dest += size;
		src += size;
	}
	src = right;
	while(src > middle){
		memcpy(dest, src, size);
		dest += size;
		src -= size;
	}

	dest = left;
	left = _msort_buf;
	right = _msort_buf + last * size;
	middle = left + (last / 2) * size;
	while(left <= right){
		if((left <= middle) && ((*comp)(left, right) <= 0)){
			memcpy(dest, left, size);
			left += size;
		}else{
			memcpy(dest, right, size);
			right -= size;
		}
		dest += size;
	}
}

extern void msort(uchar* base, int count, int size, int (*comp)())
{
	_msort_buf = malloc(size * count);
	_msort(base, base+(count-1)*size, size, comp);
	free(_msort_buf);
}
//===========================================================================
// Shift-JIS 偐傜 JIS 偵曄姺
//===========================================================================
extern void sjis_to_jis(uchar* c1, uchar* c2)
{
	if(*c1 >= 0xe0){ *c1 = *c1 - 0x40; }
	if(*c2 >= 0x80){ *c2 = *c2 - 1; }
	if(*c2 >= 0x9e){
		*c1 = (*c1 - 0x70) * 2;
		*c2 = *c2 - 0x7d;
	}else{
		*c1 = ((*c1 - 0x70) * 2) - 1;
		*c2 = *c2 - 0x1f;
	}
}
//===========================================================================
// JIS 偐傜 Shift-JIS 偵曄姺
//===========================================================================
extern void jis_to_sjis(uchar* c1, uchar* c2)
{
	if(*c1 & 0x01){
		*c1 = ((*c1 + 1) / 2) + 0x70;
		*c2 = *c2 + 0x1f;
	}else{
		*c1 = (*c1 / 2) + 0x70;
		*c2 = *c2 + 0x7d;
	}
	if(*c1 >= 0xa0){ *c1 = *c1 + 0x40; }
	if(*c2 >= 0x7f){ *c2 = *c2 + 0x01; }
}
//===========================================================================
// 暥帤僐乕僪庢摼乛奿擺
//===========================================================================
extern ushort get_chr(uchar** pp)
{
	uchar* p = *pp;
	ushort ch;

	if(*p == '\0'){ return '\0'; }

	if(ISKANJI(*p)){
		ch = (p[0] << 8) | p[1];
		(*pp) += 2;
	}else{
		ch = *p;
		(*pp)++;
	}
	return ch;
}

extern void set_chr(uchar** pp, ushort ch)
{
	if(ch > 255){
		*(*pp + 0) = ch >> 8;
		*(*pp + 1) = ch & 0xff;
		(*pp) += 2;
	}else{
		**pp = ch;
		(*pp)++;
	}
}

extern ushort peek_chr(uchar* p)
{
	return ISKANJI(*p) ? (p[0] << 8) | p[1] : *p;
}

extern void _set_chr(uchar* p, ushort ch)
{
	if(ch > 255){
		*(p + 0) = ch >> 8;
		*(p + 1) = ch & 0xff;
	}else{
		*p = ch;
	}
}
//===========================================================================
// 暥帤庬僥乕僽儖偺弶婜壔
//===========================================================================
extern void token_init()
{
	int i;

	memset(char_type, 0, sizeof(char_type));
	for (i = 'a'; i <= 'z'; i++) char_type[i] |= C_LOWER | C_SET;
	for (i = 'A'; i <= 'Z'; i++) char_type[i] |= C_UPPER | C_SET;
	for (i = '0'; i <= '9'; i++) char_type[i] |= C_DIGIT | C_HEX;
	for (i = 'a'; i <= 'f'; i++) char_type[i] |= C_HEX;
	for (i = 'A'; i <= 'F'; i++) char_type[i] |= C_HEX;

	// GBK编码的汉字范围
	// 范围：0x8140 - 0xFEFE，排除0x7F
	for (i = 0x81; i <= 0xFE; i++) {
		if (i != 0x7F) {
			char_type[i] |= C_KANJI;  // 对应的汉字类别标识
		}
	}

	// GBK编码的全角字符范围（包括全角标点符号等）
	// 范围：0xA1A1 - 0xA9FE（全角符号和标点）
	for (i = 0xA1; i <= 0xA9; i++) {
		char_type[i] |= C_KANA;  // 对应的全角字符类别标识
	}

	char_type['_'] |= C_SET;
	char_type[' '] |= C_BLANK;
	char_type['\t'] |= C_BLANK;
	char_type['\r'] |= C_BLANK;
	char_type['\n'] |= C_BLANK;
}
//===========================================================================
// 僩乕僋儞偺庢摼
//===========================================================================
extern uchar* get_token(uchar* buf)
{
	uchar ch, sign;
	int i;

	token[0] = '\0';
	token_type = _NULL_;
	i = 0;

	while(char_type[*buf] & C_BLANK){
		++buf;
	}

	if(*buf == '\0'){
		return buf;
	}

	if(char_type[*buf] & (C_SET|C_KANA|C_KANJI)){
		while(char_type[*buf] & (C_SET|C_DIGIT|C_KANA|C_KANJI)){
			if(char_type[*buf] & C_KANJI){
				token[i++] = *buf++;
			}
			token[i++] = *buf++;
		}
		token[i] = '\0';
		token_type = _KEYWORD_;
		return buf;
	}

	if(*buf == '"' || *buf == '\''){
		ch = *buf++;
		while(*buf != ch && *buf != '\0'){
			if(char_type[*buf] & C_KANJI){
				token[i++] = *buf++;
			}
			token[i++] = *buf++;
		}
		if(*buf != '\0'){ ++buf; }
		token[i] = '\0';
		token_type = _STRING_;
		return buf;
	}

	sign = 0;
	if(*buf == '+' || *buf == '-'){
		if(char_type[*(buf+1)] & C_DIGIT){
			sign = *buf++;
		}
	}

	if(char_type[*buf] & C_DIGIT){
		token_value = 0;
		if(*buf == '0'){
			++buf;
			if(*buf == 'x' || *buf == 'X'){
				++buf;
				while(char_type[*buf] & C_HEX){
					ch = *buf++;
					if(char_type[ch] & C_DIGIT){
						token_value = token_value * 16 + ch - '0';
					}else if(char_type[ch] & C_LOWER){
						token_value = token_value * 16 + ch - 'a' + 10;
					}else if(char_type[ch] & C_UPPER){
						token_value = token_value * 16 + ch - 'A' + 10;
					}
				}
			}else if(*buf == 'b' || *buf == 'B'){
				buf++;
				while(*buf == '0' || *buf == '1'){
					ch = *buf++;
					token_value = (token_value << 1) + (ch - '0');
				}
			}else{
				while(char_type[*buf] & C_DIGIT){
					ch = *buf++;
					token_value = token_value * 10 + ch - '0';
				}
			}
		}else{
			while(char_type[*buf] & C_DIGIT){
				ch = *buf++;
				token_value = token_value * 10 + ch - '0';
			}
		}
		if(sign == '-'){
			token_value = -token_value;
		}
		token_type = _CONST_;
		return buf;
	}

	token[0] = *buf++;
	token[1] = '\0';
	token_type = _SYMBOL_;
	return buf;
}
//===========================================================================
// 儕僾儗僀梡僉乕僶僢僼傽妋曐
//===========================================================================
extern void init_replay(int frame)
{
	free_replay();

	replay = malloc(frame * sizeof(REPLAYDATA));
	replay_size = frame;
	replay_index = 0;
	replay_count = 0;
}
//===========================================================================
// 儕僾儗僀僨乕僞偺儘乕僪
//===========================================================================
extern bool load_replay(char* file)
{
	free_replay();

	HFILE hf = pack_open(file);
	if(hf != NULL){
		uint size = pack_size(hf);
		replay = malloc(size);
		pack_read(hf, replay, size);
		pack_close(hf);

		replay_size = size / sizeof(REPLAYDATA);
		replay_count = replay_size;
		replay_index = 0;

		return true;
	}
	return false;
}
//===========================================================================
// 儕僾儗僀僨乕僞偺僙乕僽
//===========================================================================
extern bool save_replay(char* file)
{
	return pack_save(file, replay, replay_count*sizeof(REPLAYDATA));
}
//===========================================================================
// 儕僾儗僀僨乕僞偺夝曻
//===========================================================================
extern void free_replay()
{
	if(replay){
		free(replay);
		replay = NULL;
	}
	replay_size = 0;
	replay_count = 0;
	replay_index = 0;
}
//===========================================================================
// 儕僾儗僀儌乕僪偺愝掕
//===========================================================================
extern void set_replay(int mode)
{
	replay_mode = mode;
	if(replay_mode == REPLAY_STORE){
		replay_count = 0;
	}else if(replay_mode == REPLAY_LOAD){
		replay_index = 0;
	}
}
//===========================================================================
// 僉乕擖椡
//===========================================================================
extern void input_key()
{
	push_key = press_key;
	press_key = 0;

	if(replay_mode != REPLAY_LOAD){
		key_scan_num = 1 - key_scan_num;
		getkey(key_scan[key_scan_num]);
		getpoint(&gms);

		char* key = key_scan[key_scan_num];

		if(key[0x26] < 0){ press_key |= KEY_UP; }
		if(key[0x28] < 0){ press_key |= KEY_DOWN; }
		if(key[0x25] < 0){ press_key |= KEY_LEFT; }
		if(key[0x27] < 0){ press_key |= KEY_RIGHT; }
		if(key[0x0d] < 0){ press_key |= KEY_RET; }
		if(key[0x1b] < 0){ press_key |= KEY_ESC; }
		if(key[0x20] < 0){ press_key |= KEY_SPACE; }

		if(key[0x08] < 0){ press_key |= KEY_BACK; }
		if(key[0x21] < 0){ press_key |= KEY_PREV; }
		if(key[0x22] < 0){ press_key |= KEY_NEXT; }
		if(key[0x23] < 0){ press_key |= KEY_END; }
		if(key[0x24] < 0){ press_key |= KEY_HOME; }
		if(key[0x2c] < 0){ press_key |= KEY_PRTSC; }

		if(key[0x09] < 0){ press_key |= KEY_TAB; }
		if(key[0x10] < 0){ press_key |= KEY_SHIFT; }
		if(key[0x11] < 0){ press_key |= KEY_CTRL; }

//		if(key[0x01] < 0){ press_key |= KEY_LCLICK; }
//		if(key[0x02] < 0){ press_key |= KEY_RCLICK; }
		if(gms.button & MS_L){ press_key |= KEY_LCLICK; }
		if(gms.button & MS_R){ press_key |= KEY_RCLICK; }
		if(gms.button & MS_M){ press_key |= KEY_MCLICK; }
	}else{
		if(replay && (replay_index < replay_count)){
			press_key = replay[replay_index].key;
			gms.x = replay[replay_index].x;
			gms.y = replay[replay_index].y;
			replay_index++;
		}
	}

	push_key = press_key & ~push_key;

 	if((mouse_x != gms.x) || (mouse_y != gms.y)){
		key_mode = 0;
		if(hide_cursor_count == 0){
			cur_set(1);
			hide_cursor_count = CURSOR_HIDE_CNT;
		}
	}else{
		if(hide_cursor_count > 0){
			if(--hide_cursor_count == 0){
				cur_set(0);
			}
		}
	}

	mouse_ox = mouse_x;
	mouse_oy = mouse_y;
	mouse_x = gms.x;
	mouse_y = gms.y;
	mouse_over = (((uint)mouse_x < SCREEN_W) && ((uint)mouse_y < SCREEN_H));

	if(push_key & (KEY_LCLICK|KEY_RCLICK|KEY_MCLICK)){
		key_mode = 0;
	}
	if(press_key & PUSH_KEY){
		key_mode = 1;
	}

	if(replay_mode == REPLAY_STORE){
		if(replay && (replay_count < replay_size)){
			replay[replay_count].x = mouse_x;
			replay[replay_count].y = mouse_y;
			replay[replay_count].key = press_key;
			replay_count++;
		}
	}
}

extern bool is_press_key(uchar code)
{
	char* key = key_scan[key_scan_num];
	return key[code] < 0;
}

extern bool is_push_key(uchar code)
{
	char* key = key_scan[key_scan_num];
	char* old = key_scan[1-key_scan_num];
	return (key[code] & ~old[code]) < 0;
}

extern void hide_cursor_mode(bool flag)
{
	if(flag){
		hide_cursor_count = CURSOR_HIDE_CNT;
	}else{
		if(hide_cursor_count == 0){
			cur_set(1);
		}
		hide_cursor_count = -1;
	}
}
//===========================================================================
// 儅僂僗僇乕僜儖偺廳側傝敾掕
//===========================================================================
extern bool overlap(SPRITE* sp, int x, int y, uchar alpha)
{
	GINFO gi;
	uchar* p;
	int px, py;
	int w, h;
	int s, c;

	if((sp != NULL) && (sp->type == SP_ON) && rc_in(&sp->update, x, y)){
		if(sp->zx > 0 && sp->zy > 0){
			px = x - (sp->blit.right + sp->blit.left) / 2;
			py = y - (sp->blit.bottom + sp->blit.top) / 2;
			c = _cos(sp->real_rot);
			s = _sin(sp->real_rot);
			w = sp->rc.right - sp->rc.left;
			h = sp->rc.bottom - sp->rc.top;
			x = (px * c - py * s) / sp->real_zx + w / 2;
			y = (px * s + py * c) / sp->real_zy + h / 2;
			if(x >= 0 && y >= 0 && x < w && y < h){
				ginfo(sp->page, &gi);
				if(sp->rev_h){ x = w - x - 1; }
				if(sp->rev_v){ y = h - y - 1; }
				p = gi.pixel + (sp->rc.top + y) * gi.pitch + (sp->rc.left + x) * (gi.depth / 8);
				return ((gi.depth == 32 && p[3] >= alpha) ||
						(gi.depth ==  8 && p[0] >= alpha));
			}
		}
	}
	return false;
}
//===========================================================================
// 僞僗僋張棟弶婜壔
//===========================================================================
extern void task_init()
{
	int i;

	exec_prio = T_PRI_MAX;
	exec_task = NULL;

	task_total = 0;
	for(i = 0; i < T_PRI_MAX; i++){
		task_count[i] = 0;
		task_head[i] = NULL;
		task_tail[i] = NULL;
	}
	memset(task_list, 0, sizeof(task_list));

	task_time = getcount(0);
	task_frame = 0;
}
//===========================================================================
// 僞僗僋偺搊榐
//===========================================================================
extern TASK* task_begin(uchar prio, void (*fp)(), int data, int speed, int delay)
{
	TASK* task;
	TASK* prev_task;

	if(task_total >= T_MAX){
		error("僞僗僋偑嵟戝搊榐悢傪挻偊傑偟偨");
		return NULL;
	}

	for(task = task_list; task < &task_list[T_MAX]; task++){
		if(task->state & T_USE){ continue; }
		task_total++;
		task_count[prio]++;

		memset(task, 0, sizeof(TASK));
		task->state      = T_USE + prio;
		task->phase      = P_INIT;
		task->func       = fp;
		task->data       = data;
		task->speed      = speed;
		task->parent     = exec_task;
		task->begin_time = task_time;
		task->delay      = delay < 0 ? speed : delay;
		if(prio >= exec_prio){
			task->delay++;
		}
		if(task_head[prio] == NULL){
			task->prev = NULL;
			task->next = NULL;
			task_head[prio] = task;
			task_tail[prio] = task;
		}else{
			task_tail[prio]->next = task;
			task->prev = task_tail[prio];
			task->next = NULL;
			task_tail[prio] = task;
		}
		if(delay < 0){
			prev_task = exec_task;
			exec_task = task;
			(*fp)(task);
			exec_task = prev_task;
			if(task->state == T_OFF){
				return NULL;
			}
		}
		return task;
	}

	error("僞僗僋偺搊榐偵幐攕偟傑偟偨");
	return NULL;
}

extern bool task_init_call(TASK* task)
{
	if(task != NULL){
		TASK* prev = exec_task;
		exec_task = task;
		(*task->func)(task);
		exec_task = prev;
		return task->state != T_OFF;
	}
	return false;
}
//===========================================================================
// 僞僗僋偺嶍彍
//===========================================================================
extern void task_end(TASK* task)
{
	TASK* parent = task;
	uchar prio = task->state & 0x1f;
	int i;

	task_count[prio]--;
	task_total--;

	task->state = T_OFF;
	task->func = NULL;
	if(task->next == NULL){
		task_tail[prio] = task->prev;
	}else{
		task->next->prev = task->prev;
	}

	if(task->prev == NULL){
		task_head[prio] = task->next;
	}else{
		task->prev->next = task->next;
	}

	for(i = 0; i < T_PRI_MAX; i++){
		task = task_head[i];
		while(task){
			if(task->parent == parent){
				task->parent = NULL;
			}
			task = task->next;
		}
	}
}
//===========================================================================
// 搊榐偝傟偨僞僗僋僾儘僌儔儉偺幚峴
//===========================================================================
extern void task_call(uint flag)
{
	task_time = getcount(0);

	for(exec_prio = 0; exec_prio < T_PRI_MAX; exec_prio++){
		if(flag & (1 << exec_prio)){
			exec_task = task_head[exec_prio];
			while(exec_task){
				if(exec_task->pause <= 0){
					if(exec_task->delay > 0){
						exec_task->delay--;
					}else{
						exec_task->delay = exec_task->speed;
						(*exec_task->func)(exec_task);
					}
				}
				exec_task = exec_task->next;
			}
		}
	}
	task_frame++;
	exec_task = NULL;
}
//===========================================================================
// 巜掕僞僗僋傪嶍彍
//===========================================================================
extern void task_kill(TASK* task)
{
	if(task != NULL){
		if(task->phase == P_INIT){
			task_end(task);
		}else{
			task->phase = P_END;
			if(task->func){
				(*task->func)(task);
			}
		}
	}
}
//===========================================================================
// 巜掕僾儔僀僆儕僥傿偺僞僗僋傪慡偰嶍彍
//===========================================================================
extern void task_kill_prio(uint flag)
{
	TASK* task;
	TASK* next;
	int i;

	for(i = 0; i < T_PRI_MAX; i++){
		if(flag & (1 << i)){
			next = task_head[i];
			while(next){
				task = next;
				next = task->next;
				if(!(task->state & T_SAVE)){
					task_kill(task);
				}
			}
		}
	}
}
//===========================================================================
// 巜掕僞僗僋偺巕僞僗僋傪慡偰嶍彍
//===========================================================================
extern void task_kill_child(TASK* parent)
{
	TASK* task;
	TASK* next;
	int i;

	for(i = 0; i < T_PRI_MAX; i++){
		next = task_head[i];
		while(next){
			task = next;
			next = task->next;
			if((task->parent == parent) && !(task->state & T_SAVE)){
				task_kill_child(task);
				task_kill(task);
			}
		}
	}
}
//===========================================================================
// 巜掕俬俢偺僞僗僋傪慡偰嶍彍
//===========================================================================
extern void task_kill_id(uint id)
{
	TASK* task;
	TASK* next;
	int i;

	for(i = 0; i < T_PRI_MAX; i++){
		next = task_head[i];
		while(next){
			task = next;
			next = task->next;
			if((task->id == id) && !(task->state & T_SAVE)){
				task_kill(task);
			}
		}
	}
}
//===========================================================================
// 巜掕俬俢偺僞僗僋傪専嶕
//===========================================================================
extern TASK* task_find_id(uint id)
{
	TASK* task;
	TASK* next;
	int i;

	for(i = 0; i < T_PRI_MAX; i++){
		next = task_head[i];
		while(next){
			task = next;
			next = task->next;
			if(task->id == id){
				return task;
			}
		}
	}
	return false;
}
//===========================================================================
// 巜掕僞僗僋傪慡偰嶍彍
//===========================================================================
extern void task_kill_func(void (*func)())
{
	TASK* task;
	TASK* next;
	int i;

	for(i = 0; i < T_PRI_MAX; i++){
		next = task_head[i];
		while(next){
			task = next;
			next = task->next;
			if((task->func == func) && !(task->state & T_SAVE)){
				task_kill(task);
			}
		}
	}
}
//===========================================================================
// 僞僗僋偺堦帪掆巭
//===========================================================================
extern void task_pause(TASK* task)
{
	if(task != NULL){
		if(task->pause++ == 0){
			task->pause_time = task_time;
		}
	}
}

extern void task_all_pause(uint flag)
{
	TASK* task;
	int i;

	for(i = 0; i < T_PRI_MAX; i++){
		if(flag & (1 << i)){
			task = task_head[i];
			while(task){
				task_pause(task);
				task = task->next;
			}
		}
	}
}
//===========================================================================
// 僞僗僋偺幚峴嵞奐
//===========================================================================
extern void task_resume(TASK* task)
{
	if(task && (task->pause > 0)){
		if(--task->pause == 0){
			task->begin_time += task_time - task->pause_time;
		}
	}
}

extern void task_all_resume(uint flag)
{
	TASK* task;
	int i;

	for(i = 0; i < T_PRI_MAX; i++){
		if(flag & (1 << i)){
			task = task_head[i];
			while(task){
				task_resume(task);
				task = task->next;
			}
		}
	}
}
//===========================================================================
// 僞僗僋帪娫偺弶婜壔
//===========================================================================
extern void reset_tick(TASK* task)
{
	task->begin_time = task_time;
}
//===========================================================================
// 僞僗僋帪娫偺庢摼
//===========================================================================
extern uint get_tick(TASK* task)
{
	return (task_time - task->begin_time);
}
//===========================================================================
// 僞僗僋娭悢
//===========================================================================
typedef struct{
	int		(*func)();
	void*	param;
	int		param_size;
} TFUNC_DESC;

OBJ( function_task )
{
	TFUNC_DESC* w = task->work;

	switch( task->phase ){
	case 0:
		task->data = 0;
		task->phase++;
	case 1:
		if((*w->func)(w->param) != 0){
			break;
		}
		task->phase = -1;
	case -1:
		if(T_WSIZE-sizeof(TFUNC_DESC) < w->param_size){
			free(w->param);
		}
		task_end(task);
		break;
	}
}
extern TASK* task_func(int prio, int (*func)(), void* param, int size)
{
	TASK* task = task_begin(prio, function_task, NULL, 0, 0);
	if(task != NULL){
		TFUNC_DESC* work = task->work;
		work->func = func;
		work->param = param;
		work->param_size = size;
		if((param != NULL) && (size > 0)){
			if(T_WSIZE-sizeof(TFUNC_DESC) < size){
				work->param = malloc(size);
			}else{
				work->param = work + 1;
			}
			memcpy(work->param, param, size);
		}
		task_init_call(task);
	}
	return task;
}
//===========================================================================
// 儅僂僗僀儀儞僩偺弶婜壔
//===========================================================================
extern void init_event(EVLIST* ev_list)
{
	memset(ev_list, 0, sizeof(EVLIST));
}
//===========================================================================
// 儅僂僗僀儀儞僩偺搊榐
//===========================================================================
extern EVENT* set_event(EVLIST* ev_list, void (*func)(), int data)
{
	if(!ev_list || !func || (ev_list->count >= MAX_EVENT)){
		return NULL;
	}

	EVENT* ev = &ev_list->event[ev_list->count++];
	ev->id = -1;
	ev->func = func;
	ev->data = data;
	ev->timer = false;
	ev->keyboard = false;
	ev->var = ev_list->var;

	(*ev->func)(EV_INIT, ev);

	return ev;
}
//===========================================================================
// 儅僂僗僀儀儞僩偺張棟
//===========================================================================
extern void call_event(EVLIST* ev_list)
{
	EVENT* ev;
	EVENT* last;
	EVENT* left;
	EVENT* right;
	uint decide;
	uint cancel;
	int id;
	int i;

	last = ev_list->last;
	if(ev_list->hold == 0){
		if(push_key & KEY_CURSOR){
			id = -1;
			if((last != NULL) && last->keyboard){
				id = (*last->func)(EV_CURSOR, last);
			}
			if(id == -1){
				for(i = ev_list->count-1; i >= 0; i--){
					ev = &ev_list->event[i];
					id = (*ev->func)(EV_CURSOR, ev);
					if(id >= 0){ break; }
				}
			}
		}
		// 僀儀儞僩斖埻撪偵儅僂僗僇乕僜儖偑偁傞偐挷傋傞
		for(i = ev_list->count-1; i >= 0; i--){
			ev = &ev_list->event[i];
			id = (*ev->func)(EV_CHECK, ev);
			if(id >= 0){ break; }
		}
		if(i < 0){
			ev = NULL;
			id = -1;
		}
		// 儅僂僗僇乕僜儖偑僀儀儞僩斖埻偵恑擖丒戅弌偟偨儊僢僙乕僕傪憲傞
		if((ev != last) || (ev && (ev->id != id))){
			if(last != NULL){
				(*last->func)(EV_LEAVE, last);
				last->id = -1;
			}
			if(ev != NULL){
				ev->id = id;
				(*ev->func)(EV_ENTER, ev);
			}
			ev_list->last = last = ev;
		}
		// 儅僂僗偑僋儕僢僋偝傟偨儊僢僙乕僕傪憲傞
		if(last != NULL){
			decide = push_key & KEY_LCLICK;
			cancel = push_key & KEY_RCLICK;
			if(last->keyboard){
				decide |= push_key & KEY_RET;
				cancel |= push_key & KEY_ESC;
			}
			if(decide){
				if((*last->func)(EV_LB_DOWN, last)){ ev_list->hold++; }
				ev_list->left = last;
			}else if(cancel){
				if((*last->func)(EV_RB_DOWN, last)){ ev_list->hold++; }
				ev_list->right = last;
			}
		}
	}else{
		// 儅僂僗儃僞儞偑棧偝傟偨儊僢僙乕僕傪憲傞
		left  = ev_list->left;
		right = ev_list->right;
		decide = (key_scan[key_scan_num][0x01] < 0);
		cancel = (key_scan[key_scan_num][0x02] < 0);
		if(last->keyboard){
			decide |= press_key & KEY_RET;
			cancel |= press_key & KEY_ESC;
		}
		if(left && !decide){
			if((*left->func)(EV_LB_UP, left)){ ev_list->hold--; }
			ev_list->left = left = NULL;
		}else if(right && !cancel){
			if((*right->func)(EV_RB_UP, right)){ ev_list->hold--; }
			ev_list->right = right = NULL;
		}
		// 僪儔僢僌儊僢僙乕僕傪憲傞
		if(left && ((mouse_x != mouse_ox) || (mouse_y != mouse_oy))){
			(*left->func)(EV_DRAG, left);
		}
	}

	// 僞僀儅乕僀儀儞僩
	for(i = 0; i < ev_list->count; i++){
		ev = &ev_list->event[i];
		if(ev->timer){
			(*ev->func)(EV_TIMER, ev);
		}
	}
}
//===========================================================================
// 儅僂僗僀儀儞僩偺僉儍儞僙儖
//===========================================================================
extern void cancel_event(EVLIST* ev_list)
{
	EVENT* last;
	EVENT* left;
	EVENT* right;

	if(ev_list->hold != 0){
		// 儅僂僗儃僞儞偑棧偝傟偨儊僢僙乕僕傪憲傞
		left  = ev_list->left;
		right = ev_list->right;
		if(left != NULL){
			if((*left->func)(EV_LB_UP, left)){ ev_list->hold--; }
			ev_list->left = NULL;
		}else if(right != NULL){
			if((*right->func)(EV_RB_UP, right)){ ev_list->hold--; }
			ev_list->right = NULL;
		}
	}
	// 儅僂僗僇乕僜儖偑僀儀儞僩斖埻偐傜戅弌偟偨儊僢僙乕僕傪憲傞
	last = ev_list->last;
	if(last != NULL){
		(*last->func)(EV_LEAVE, last);
		ev_list->last = NULL;
	}
}
//===========================================================================
// 儅僂僗僀儀儞僩偺斖埻撪偐挷傋傞
//===========================================================================
extern bool is_event_area(EVENT* ev)
{
	int id;

	if((ev != NULL) && (ev->func != NULL)){
		id = (*ev->func)(EV_CHECK, ev);
		return id == ev->id;
	}
	return false;
}
//===========================================================================
// 僗僾儔僀僩斈梡
//===========================================================================
BLTMODE _sp_blt;

extern void sp_blt(BLTMODE* pbm)
{
	if(pbm == NULL){
		_sp_blt.mode = BLT_NORMAL;
	}else{
		memcpy(&_sp_blt, pbm, sizeof(BLTMODE));
	}
}

extern SPRITE* sp_def(int pri, int type, int x, int y, int page, RECT* prc)
{
	return sp_set(pri, type, x, y, page, prc, &_sp_blt);
}

extern SPRITE* sp_defc(int pri, int type, int x, int y, int page, RECT* prc, int cx, int cy, SPRITE* parent)
{
	SPRITE* sp = sp_set(pri, type, x, y, page, prc, &_sp_blt);
	sp->cx = cx;
	sp->cy = cy;
	if(parent != NULL){ sp_link(parent, sp); }
	return sp;
}

extern SPRITE* clone_sprite(SPRITE* sp_src)
{
	if(sp_src == NULL){ return NULL; }

	SPRITE* sp_dst = sp_set(sp_src->priority, sp_src->type,
		sp_src->x, sp_src->y, sp_src->page, &sp_src->rc, &sp_src->bm);
	sp_dst->rev_h = sp_src->rev_h;
	sp_dst->rev_v = sp_src->rev_v;
	sp_dst->fix = sp_src->fix;
	sp_dst->sort = sp_src->sort;
	sp_dst->cx = sp_src->cx;
	sp_dst->cy = sp_src->cy;
	sp_dst->zx = sp_src->zx;
	sp_dst->zy = sp_src->zy;
	sp_dst->rot = sp_src->rot;
//	SPRITE* child = sp_src->child;
//	while(child != NULL){
//		sp_link(sp_dst, clone_sprite(child));
//		child = child->next;
//	}
	return sp_dst;
}

extern int sp_hit(SPRITE** sp, int count)
{
	int i;

	if(mouse_over && (sp != NULL)){
		for(i = count - 1; i >= 0; i--){
			if((sp[i] != NULL) && (sp[i]->type == SP_ON)){
				if(rc_in(&sp[i]->update, mouse_x, mouse_y)){
					return i;
				}
			}
		}
	}
	return -1;
}

extern void sp_visible(SPRITE* sp, bool flag)
{
	if(sp == NULL){ return; }
	sp->type = flag ? SP_ON : SP_OFF;
	sp->flag = SP_ONCE;
}

extern void sp_on(SPRITE* sp)
{
	if(sp == NULL){ return; }
	sp->type = SP_ON;
	sp->flag = SP_ONCE;
}

extern void sp_off(SPRITE* sp)
{
	if(sp == NULL){ return; }
	sp->type = SP_OFF;
}

extern void sp_chg_rev(SPRITE* sp, bool rev_h, bool rev_v)
{
	if(sp == NULL){ return; }
	sp->rev_h = rev_h;
	sp->rev_v = rev_v;
	sp->flag = SP_ONCE;
}

extern void sp_chg_page(SPRITE* sp, int page)
{
	if(sp == NULL){ return; }
	sp->page = page;
	sp->flag = SP_ONCE;
}

extern void sp_chg_xy(SPRITE* sp, int x, int y)
{
	if(sp == NULL){ return; }
	sp->x = x;
	sp->y = y;
	sp->flag = SP_ONCE;
}

extern void sp_add_xy(SPRITE* sp, int x, int y)
{
	if(sp == NULL){ return; }
	sp->x += x;
	sp->y += y;
	sp->flag = SP_ONCE;
}

extern void sp_chg_center(SPRITE* sp, int x, int y)
{
	if(sp == NULL){ return; }
	sp->cx = x;
	sp->cy = y;
	sp->flag = SP_ONCE;
}

extern void sp_chg_rc(SPRITE* sp, RECT* prc)
{
	if(sp == NULL){ return; }
	rc_copy(&sp->rc, prc);
	sp->flag = SP_ONCE;
}

extern void sp_chg_rc2(SPRITE* sp, int x, int y)
{
	if(sp == NULL){ return; }
	rc_move(&sp->rc, x-sp->rc.left, y-sp->rc.top);
	sp->flag = SP_ONCE;
}

extern void sp_add_rc(SPRITE* sp, int ofx, int ofy)
{
	if(sp == NULL){ return; }
	rc_move(&sp->rc, ofx, ofy);
	sp->flag = SP_ONCE;
}

extern void sp_chg_zoom(SPRITE* sp, int zx, int zy)
{
	if(sp == NULL){ return; }
	sp->zx = zx;
	sp->zy = zy;
	sp->flag = SP_ONCE;
}

extern void sp_add_zoom(SPRITE* sp, int zx, int zy)
{
	if(sp == NULL){ return; }
	sp->zx += zx;
	sp->zy += zy;
	sp->flag = SP_ONCE;
}

extern void sp_chg_rot(SPRITE* sp, int rot)
{
	if(sp == NULL){ return; }
	sp->rot = rot;
	sp->flag = SP_ONCE;
}

extern void sp_add_rot(SPRITE* sp, int rot)
{
	if(sp == NULL){ return; }
	sp->rot += rot;
	sp->flag = SP_ONCE;
}

extern void sp_chg_blt(SPRITE* sp, BLTMODE* blt)
{
	if(sp == NULL || blt == NULL){ return; }
	memcpy(&sp->bm, blt, sizeof(BLTMODE));
	sp->flag = SP_ONCE;
}

extern void sp_chg_level(SPRITE* sp, uchar level)
{
	if(sp == NULL){ return; }
	sp->bm.level = level;
	sp->flag = SP_ONCE;
}

extern void sp_chg_range(SPRITE* sp, uchar range)
{
	if(sp == NULL){ return; }
	sp->bm.range = range;
	sp->flag = SP_ONCE;
}

extern void sp_chg_color(SPRITE* sp, uint col)
{
	if(sp == NULL){ return; }
	sp->bm.color = col;
	sp->flag = SP_ONCE;
}

extern void sp_chg_sort(SPRITE* sp, int sort)
{
	if(sp == NULL){ return; }
	sp->sort = sort;
}

extern void sp_update(SPRITE* sp)
{
	if(sp == NULL){ return; }
	sp->flag = SP_ONCE;
}

extern void sp_cursor(SPRITE* sp)
{
	if(sp == NULL){ return; }
	mouse_x = sp->update.left + rc_width(&sp->update)/2;
	mouse_y = sp->update.top + rc_height(&sp->update)/2;
	cur_setpos(mouse_x, mouse_y);
}
//===========================================================================
// 僗僾儔僀僩僔乕働儞僗偺嶌惉
//===========================================================================
extern SPRSEQ* seq_create()
{
	SPRSEQ* seq = malloc(sizeof(SPRSEQ));
	if(seq != NULL){
		memset(seq, 0, sizeof(SPRSEQ));
		seq->ref = 1;
	}
	return seq;
}
//===========================================================================
// 僗僾儔僀僩僔乕働儞僗偺嶲徠
//===========================================================================
extern void seq_add(SPRSEQ* seq)
{
	if(seq != NULL){
		++seq->ref;
	}
}
//===========================================================================
// 僗僾儔僀僩僔乕働儞僗偺夝曻
//===========================================================================
extern void seq_release(SPRSEQ* seq)
{
	int i;

	if(seq != NULL){
		if(seq->ref == 1){
			for(i = 0; i < SEQ_MAX; i++){
				if(seq->key[i] != NULL){ free(seq->key[i]); }
			}
			free(seq);
		}else{
			--seq->ref;
		}
	}
}
//===========================================================================
// 僗僾儔僀僩僔乕働儞僗偺愝掕
//===========================================================================
extern void seq_set_param(SPRSEQ* seq, uint index, int param, uint time)
{
	KEYFRAME* key;
	int count;

	if((seq != NULL) && (index < SEQ_MAX)){
		key = seq->key[index];
		if(key == NULL){
			seq->key_max[index] = 2;
			key = malloc(sizeof(KEYFRAME) * seq->key_max[index]);
			key[0].param = 0.f;
			key[0].time = 0.f;
			seq->key[index] = key;
			seq->key_count[index] = 1;
		}else if(seq->key_count[index] >= seq->key_max[index]){
			seq->key_max[index] *= 2;
			key = malloc(sizeof(KEYFRAME) * seq->key_max[index]);
			memcpy(key, seq->key[index], sizeof(KEYFRAME) * seq->key_count[index]);
			free(seq->key[index]);
			seq->key[index] = key;
		}
		count = seq->key_count[index]++;
		key[count].param = key[count-1].param + (float)param;
		key[count].time = key[count-1].time + (float)time/1000.f;
	}
}
//===========================================================================
// 僗僾儔僀僩僔乕働儞僗偺愝掕
//===========================================================================
extern void seq_set_text(SPRSEQ* seq, uchar* text)
{
	int param[2];
	int param_count = 0;
	int index = -1;

	text = get_token(text);
	while(token_type != _NULL_){
		if(token_type == _KEYWORD_){
			index = -1;
			param_count = 0;
			if     (!strcmpi(token, "x"  )){ index = SEQ_X; }
			else if(!strcmpi(token, "y"  )){ index = SEQ_Y; }
			else if(!strcmpi(token, "zx" )){ index = SEQ_ZX; }
			else if(!strcmpi(token, "zy" )){ index = SEQ_ZY; }
			else if(!strcmpi(token, "rot")){ index = SEQ_ROT; }
			else if(!strcmpi(token, "lv" )){ index = SEQ_LV; }
		}else if(token_type == _CONST_){
			param[param_count] = token_value;
			param_count++;
			if(param_count == 2){
				seq_set_param(seq, index, param[0], param[1]);
				param_count = 0;
			}
		}else{
			if((token_type != _SYMBOL_) ||
				((token[0] != ',') && (token[0] != ':') && (token[0] != ';'))){
				index = -1;
				param_count = 0;
			}
		}
		text = get_token(text);
	}
}
//===========================================================================
// 僗僾儔僀僩僔乕働儞僗偺廔椆帪娫傪庢摼
//===========================================================================
extern uint seq_get_time(SPRSEQ* seq)
{
	uint last_time = 0;
	uint time;
	int i;

	if(seq != NULL){
		for(i = 0; i < SEQ_MAX; i++){
			if(seq->key[i]){
				time = (int)(seq->key[i][seq->key_count[i]-1].time * 1000.f + 0.5f);
				if(last_time < time){
					last_time = time;
				}
			}
		}
	}
	return last_time;
}
//===========================================================================
// 僗僾儔僀僩僔乕働儞僗僞僗僋
//===========================================================================
typedef struct{
	SPRITE*		sp;
	SPRSEQ*		seq;
	bool		pause;
	bool		tcb;
	uint		time;
	int			param[SEQ_MAX];
} SPRSEQ_WORK;

OBJ( seq_play_task )
{
	SPRSEQ_WORK* w = task->work;
	uint tick;
	bool update;
	int param;
	int offset;
	int mode;
	int i;

	switch( task->phase ){
	case 0:
		if(w->pause){
			task_pause(task->parent);
		}
		seq_play_count++;
		seq_add(w->seq);
		task->data = 0;
		task->phase++;
		break;
	case 1:
		if(task->data == 0){
			w->time = seq_get_time(w->seq);
			for(i = 0; i < SEQ_MAX; i++){
				w->param[i] = 0;
			}
			reset_tick(task);
			task->data = 1;
		}
		tick = min(get_tick(task), w->time);
		update = false;
		for(i = 0; i < SEQ_MAX; i++){
			if(w->seq->key[i] != NULL){
				mode = KF_LINEAR;
				if(w->tcb && (w->seq->key_count[i] > 2)){
					mode = KF_TCB;
				}
				param = w->param[i];
				w->param[i] = (int)kf_getkey(w->seq->key[i], w->seq->key_count[i], tick/1000.0f, mode);
				offset = w->param[i] - param;
				if(offset != 0){
					switch( i ){
					case SEQ_X: w->sp->x += offset; break;
					case SEQ_Y: w->sp->y += offset; break;
					case SEQ_ZX: w->sp->zx += offset; break;
					case SEQ_ZY: w->sp->zy += offset; break;
					case SEQ_ROT: w->sp->rot += offset; break;
					case SEQ_LV: w->sp->bm.level = limit(w->sp->bm.level+offset,0,255); break;
					}
					update = true;
				}
			}
		}
		if(update){ sp_update(w->sp); }
		if(tick < w->time){ break; }
		task->phase = -1;
	case -1:
		seq_release(w->seq);
		if(w->pause){
			task_resume(task->parent);
		}
		seq_play_count--;
		task_end(task);
		break;
	}
}
//===========================================================================
// 僗僾儔僀僩僔乕働儞僗偺嵞惗
//===========================================================================
extern void seq_play(SPRSEQ* seq, SPRITE* sp, bool tcb, bool pause)
{
	TASK* task = task_begin(4, seq_play_task, NULL, 0, 0);
	if(task != NULL){
		SPRSEQ_WORK* w = task->work;
		w->sp = sp;
		w->seq = seq;
		w->tcb = tcb;
		w->pause = pause;
		task_init_call(task);
	}
}
//===========================================================================
// 嬮宍暆偺庢摼
//===========================================================================
extern int rc_width(RECT* rc)
{
	return rc->right - rc->left;
}

extern int rc_height(RECT* rc)
{
	return rc->bottom - rc->top;
}
//===========================================================================
// 柍岠嬮宍
//===========================================================================
extern void update_rect(int x, int y, int w, int h)
{
	RECT rc;
	rc_box(&rc, x, y, w, h);
	rc_union(&rc_update, &rc_update, &rc);
}
//===========================================================================
// 儊僀儞儖乕僾
//===========================================================================
void task_loop()
{
	RECT rc;
	uint time;
	uint tick;
	uint interval;
	int skip;

	tick = getcount(0);
	time = tick;
	skip = 0;
	frame_count = 0;
	exit_task = false;

	while(!exit_task){
		input_key();
		task_call(T_ALL);

		if(!frame_skip || skip == 0){
			// 昤夋張棟
			sp_sort(sort_flag, 2);
			sp_draw(disp_flag, &rc);
			rc_union(&rc, &rc, &rc_update);
			update(&rc);
			rc_empty(&rc_update);
			// 僼儗乕儉僇僂儞僩
			++frame_count;
			interval = getcount(0) - time;
			if(interval >= 1000){
//				log_out("%d", frame_count * 1000 / interval);
				frame_count = 0;
				time += interval;
			}
			interval = getcount(0) - tick;
			if(frame_skip && (interval > FRAMETIME)){
				// 僗僉僢僾悢寁嶼
				skip = interval / FRAMETIME;
				if(skip >= 60){
					skip = 0;
					tick = getcount(0);
				}else if(skip > FRAMESKIP){
					skip = FRAMESKIP;
				}
				tick += skip * FRAMETIME;
			}else{
				// 僞僀儈儞僌挷惍
				while(interval + 5 < FRAMETIME){
					sleep(1);
					interval = getcount(0) - tick;
				}
				while(interval < FRAMETIME){
					interval = getcount(0) - tick;
				}
				tick += FRAMETIME;
			}
		}else{
			// 僼儗乕儉僗僉僢僾
			skip--;
		}
	}
}

extern void task_main_loop()
{
	task_loop();
	task_kill_prio(T_ALL);
}

extern void task_sub_loop()
{
	TASK* task = exec_task;
	int prio = exec_prio;
	uint time = task_time;
	uint tick = getcount(0);
	task_loop();
	setcount(0, tick);
	task_time = time;
	exec_prio = prio;
	exec_task = task;
	exit_task = false;
}
