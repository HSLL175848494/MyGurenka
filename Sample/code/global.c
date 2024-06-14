#include "global.h"
#include "game.c"
#include "anime.c"
#include "text.c"
#include "calender.c"
#include "gurenka.c"

uchar get_music_volume()
{
	uchar buf[256];
	uchar* p;

	if(game_mode == MODE_MUSIC){
		// MusicPlayer‚Å‚Ìƒ{ƒŠƒ…[ƒ€’l
		return 100;
	}
	strcpy(buf, music_name);
	if(p = mbschr(buf, '_')){
		*p = '\0';
		// sys_ ‚Ån‚Ü‚éBGM‚Ìƒ{ƒŠƒ…[ƒ€’l
		if(strcmpi(buf, "sys") == 0){ return 100; }
		// btl_ ‚Ån‚Ü‚éBGM‚Ìƒ{ƒŠƒ…[ƒ€’l
		if(strcmpi(buf, "btl") == 0){ return 100; }
	}
	// ‚»‚Ì‘¼BGM‚Ìƒ{ƒŠƒ…[ƒ€’l
	return 100;
}
//===========================================================================
// ƒVƒXƒeƒ€‚Ì‰Šú‰»
//===========================================================================
extern void init_system()
{
	ginit();
	sp_init(SP_ALL);
	init_image_lib();
	init_text_lib();
	init_anime_lib();
	galloc(BACKSCREEN, SCREEN_W, SCREEN_H, 24);
	scroll_x = scroll_y = 0;
	disp_flag = SP_ALL;
}
//===========================================================================
// ƒVƒXƒeƒ€‚ÌŒãn––
//===========================================================================
extern void free_system()
{
	free_anime_lib();
	free_text_lib();
	free_image_lib();
}
//===========================================================================
// ƒXƒNƒŠƒvƒg‚ÌƒeƒLƒXƒgŠJnˆÊ’u‚ğ‰Šú‰»
//===========================================================================
void init_script_literal()
{
	uchar buf[256];
	HFILE fp;
	int count, total;
	int i;

	if(script_dat_cnt <= 0){ return; }

	total = 0;
	for(i = 0; i < script_dat_cnt; i++){
		sprintf(buf, DIR_SCRIPT "%s.bin", script_dat[i].file);
		if(fp = pack_open(buf)){
			pack_read(fp, buf, 8);
			if(memcmp(buf, "ESCR1_00", 8) == 0){
				pack_read(fp, &count, sizeof(count));
			}else{
				count = 0;
			}
			pack_close(fp);
			adv_literal[i] = total;
			total += count;
		}else{
			adv_literal[i] = total;
		}
	}
	adv_literal[i] = total;
}
//===========================================================================
// ƒTƒEƒ“ƒhƒGƒtƒFƒNƒgƒtƒ@ƒCƒ‹‚Ìƒ[ƒh
//===========================================================================
void load_soundfx()
{
	uchar buf[256];
	HFILE fp;
	int i;

	if(sfx_dat_cnt <= 0){ return; }

	soundfx_loaded = malloc(sfx_dat_cnt * sizeof(uchar));
	soundfx_data = malloc(sfx_dat_cnt * sizeof(SOUNDFX));

	memset(soundfx_loaded, 0, sfx_dat_cnt * sizeof(uchar));
	memset(soundfx_data, 0, sfx_dat_cnt * sizeof(SOUNDFX));

	for(i = 1; i < sfx_dat_cnt; i++){
		sprintf(buf, DIR_SFX "%s.sfx", sfx_dat[i].file);
		if(fp = pack_open(buf)){
			pack_read(fp, &soundfx_data[i], sizeof(SOUNDFX));
			pack_close(fp);
			soundfx_loaded[i] = true;
		}else{
			sprintf(buf, DIR_SFX "%s.sfx ‚Ì“Ç‚İ‚İ‚É¸”s‚µ‚Ü‚µ‚½B", sfx_dat[i].file);
			mbox(MB_OK, "error!", buf);
		}
	}
}
//===========================================================================
// ƒTƒEƒ“ƒhƒGƒtƒFƒNƒg‚Ì“K—p
//===========================================================================
extern bool apply_soundfx(int track, ushort sfx_num)
{
	SOUNDFX* soundfx;

	if((sfx_num == 0) || (sfx_num >= sfx_dat_cnt)){
		return false;
	}
	if(!soundfx_loaded[sfx_num]){
		return false;
	}

	soundfx = &soundfx_data[sfx_num];

	if(soundfx->use_chorus){
		snd_setchorus(track, &soundfx->chorus);
	}
	if(soundfx->use_compressor){
		snd_setcompressor(track, &soundfx->compressor);
	}
	if(soundfx->use_distortion){
		snd_setdistortion(track, &soundfx->distortion);
	}
	if(soundfx->use_echo){
		snd_setecho(track, &soundfx->echo);
	}
	if(soundfx->use_flanger){
		snd_setflanger(track, &soundfx->flanger);
	}
	if(soundfx->use_gargle){
		snd_setgargle(track, &soundfx->gargle);
	}
	if(soundfx->use_param_eq){
		snd_setparameq(track, &soundfx->param_eq);
	}
	if(soundfx->use_reverb){
		snd_setreverb(track, &soundfx->reverb);
	}
	return true;
}
//===========================================================================
//  ƒfƒBƒŒƒCŠÔ‚Ìæ“¾
//===========================================================================
extern uint get_soundfx_delay(ushort sfx_num)
{
	SOUNDFX* soundfx;
	float delay;
	float time = 0.0f;

	if((sfx_num == 0) || (sfx_num >= sfx_dat_cnt)){
		return 0;
	}
	if(!soundfx_loaded[sfx_num]){
		return 0;
	}

	soundfx = &soundfx_data[sfx_num];

	if(soundfx->use_chorus){
		delay = soundfx->chorus.delay;
		if(time < delay){ time = delay; }
	}
	if(soundfx->use_compressor){
		delay = soundfx->compressor.release;
		if(time < delay){ time = delay; }
	}
	if(soundfx->use_echo){
		delay = soundfx->echo.left_delay;
		if(time < delay){ time = delay; }
		delay = soundfx->echo.right_delay;
		if(time < delay){ time = delay; }
	}
	if(soundfx->use_flanger){
		delay = soundfx->flanger.delay;
		if(time < delay){ time = delay; }
	}
	if(soundfx->use_reverb){
		delay = soundfx->reverb.reverb_time * 0.5;
		if(time < delay){ time = delay; }
	}
	return (uint)time;
}
//===========================================================================
// ƒ{ƒŠƒ…[ƒ€æ“¾
//===========================================================================
extern uchar get_volume(int track)
{
	int vol;

	if(track >= SND_BGM && track < SND_BGM+SND_BGM_MAX){
		vol = (inner_music_vol * system.music_vol * system.master_vol) / 10000;
		return limit((vol*fade_volume[track])/100, 0, 100);
	}
	if(track >= SND_AMB && track < SND_AMB+SND_AMB_MAX){
		vol = (inner_ambient_vol * system.ambient_vol * system.master_vol) / 10000;
		return limit((vol*fade_volume[track])/100, 0, 100);
	}
	if(track >= SND_EFE && track < SND_EFE+SND_EFE_MAX){
		vol = (inner_sound_vol * system.sound_vol * system.master_vol) / 10000;
		return limit((vol*fade_volume[track])/100, 0, 100);
	}
	if(track >= SND_SYS && track < SND_SYS+SND_SYS_MAX){
		vol = (system.sound_vol * system.master_vol) / 100;
		return limit((vol*fade_volume[track])/100, 0, 100);
	}
	if(track >= SND_FRE && track < SND_FRE+SND_FRE_MAX){
		vol = (system.sound_vol * system.master_vol) / 100;
		return limit((vol*fade_volume[track])/100, 0, 100);
	}
	if(track >= SND_VOC && track < SND_VOC+SND_VOC_MAX){
		vol = voice_group[limit(voice_num[track-SND_VOC]/VOC_MASK, 0, 99)];
		vol = (inner_voice_vol * system.voice_vol * system.chars_vol[vol] * system.master_vol) / 1000000;
		return limit((vol*fade_volume[track])/100, 0, 100);
	}
	return 0;
}
//===========================================================================
// ƒ{ƒŠƒ…[ƒ€ƒtƒF[ƒh
//===========================================================================
typedef struct{
	uchar	id;
	uchar	track;
	uchar	volume0;
	uchar	volume1;
	uchar	fixed_inner_volume;
	uchar	inner_volume[4];
	uchar	dispose_sound;
	uint	fade_time;
} FADEWORK;

OBJ( volume_fade_task )
{
	FADEWORK* w = task->work;
	uint tick;

	switch( task->phase ){
	case 0:
		if(w->fixed_inner_volume){
			w->inner_volume[0] = inner_music_vol;
			w->inner_volume[1] = inner_ambient_vol;
			w->inner_volume[2] = inner_voice_vol;
			w->inner_volume[3] = inner_sound_vol;
		}
		task->state |= T_SAVE;
		task->phase++;
	case 1:
		if(snd_isplay(w->track)){
			tick = min(get_tick(task), w->fade_time);
			if(w->fade_time == 0){
				fade_volume[w->track] = w->volume1;
			}else{
				fade_volume[w->track] = flat_speed(
					w->volume0, w->volume1, (float)tick/w->fade_time);
			}
			if(w->fixed_inner_volume){
				SWAP(w->inner_volume[0], inner_music_vol);
				SWAP(w->inner_volume[1], inner_ambient_vol);
				SWAP(w->inner_volume[2], inner_voice_vol);
				SWAP(w->inner_volume[3], inner_sound_vol);
			}
			snd_setvol(w->track, get_volume(w->track));
			if(w->fixed_inner_volume){
				SWAP(w->inner_volume[0], inner_music_vol);
				SWAP(w->inner_volume[1], inner_ambient_vol);
				SWAP(w->inner_volume[2], inner_voice_vol);
				SWAP(w->inner_volume[3], inner_sound_vol);
			}
			if(tick < w->fade_time){ break; }
		}
		task->data = 0;
		task->phase++;
	case 2:
/*
		if(w->dispose_sound && snd_isplay(w->track)){
			if(task->data == 0){
				snd_setvol(w->track, 0);
				reset_tick(task);
				task->data = 1;
			}
			if(get_tick(task) < 50){ break; }
		}
*/
		task->phase = -1;
	case -1:
		fade_volume[w->track] = w->volume1;
		if(w->dispose_sound){
			snd_setvol(w->track, 0);
			if(snd_isplay(w->track)){ sleep(50); }
			snd_free(w->track);
			fade_volume[w->track] = 100;
		}
		fade_task[w->track] = NULL;
		task_end(task);
		break;
	}
}

extern TASK* volume_fade(int track, uchar volume0, uchar volume1, uint fade_time)
{
	int n;

	if(track < 0){ return NULL; }

	if((track >= SND_BGM) && (track < SND_BGM+SND_BGM_MAX)){
		n = get_music_volume();
		volume0 = volume0 * n / 100;
		volume1 = volume1 * n / 100;
	}

	task_kill(fade_task[track]);
	fade_task[track] = task_begin(1, volume_fade_task, NULL, 0, 0);
	if(fade_task[track] != NULL){
		FADEWORK* work = fade_task[track]->work;
		work->track = track;
		work->fade_time = fade_time;
		work->volume0 = volume0;
		work->volume1 = volume1;
		work->fixed_inner_volume = (volume1 == 0);
		work->dispose_sound = work->fixed_inner_volume;
		task_init_call(fade_task[track]);
	}
	return fade_task[track];
}
//===========================================================================
// ƒ†[ƒU[—pƒ{ƒŠƒ…[ƒ€ƒtƒF[ƒh
//===========================================================================
OBJ( user_volume_fade_task )
{
	FADEWORK* w = task->work;
	uint tick;
	int n, vol;

	switch( task->phase ){
	case 0:
		task->state |= T_SAVE;
		task->phase++;
	case 1:
		tick = min(get_tick(task), w->fade_time);
		if(w->fade_time == 0){
			vol = w->volume1;
		}else{
			vol = flat_speed(w->volume0, w->volume1, (float)tick/w->fade_time);
		}
		switch( w->id ){
		case 0: // BGM
			inner_music_vol = vol;
			n = SND_BGM + music_play_track;
			snd_setvol(n, get_volume(n));
			break;
		case 1: // ŠÂ‹«‰¹
			inner_ambient_vol = vol;
			n = SND_AMB + ambient_play_track;
			snd_setvol(n, get_volume(n));
			break;
		case 2: // Œø‰Ê‰¹
			inner_sound_vol = vol;
			for(n = SND_EFE; n < SND_EFE+SND_EFE_MAX; n++){
				snd_setvol(n, get_volume(n));
			}
			break;
		case 3: // ƒLƒƒƒ‰‰¹º
			inner_voice_vol = vol;
			for(n = SND_VOC; n < SND_VOC+SND_VOC_MAX; n++){
				snd_setvol(n, get_volume(n));
			}
			break;
		}
		if(tick < w->fade_time){ break; }
		task->phase = -1;
	case -1:
		user_fade_task[w->id] = NULL;
		task_end(task);
		break;
	}
}

extern TASK* user_volume_fade(uchar id, uchar volume0, uchar volume1, uint fade_time)
{
	if(id < 0){ return NULL; }

	task_kill(user_fade_task[id]);
	user_fade_task[id] = task_begin(0, user_volume_fade_task, NULL, 0, 0);
	if(user_fade_task[id] != NULL){
		FADEWORK* work = user_fade_task[id]->work;
		work->id = id;
		work->volume0 = volume0;
		work->volume1 = volume1;
		work->fade_time = fade_time;
		task_init_call(user_fade_task[id]);
	}
	return user_fade_task[id];
}

extern void init_user_volume()
{
	user_volume_fade(0, 0, 100, 0);
	user_volume_fade(1, 0, 100, 0);
	user_volume_fade(2, 0, 100, 0);
	user_volume_fade(3, 0, 100, 0);
}
//===========================================================================
// ƒXƒgƒŠ[ƒ€Ä¶
//===========================================================================
void play_stream(int track, uchar* file, uint loop, uint begin_time, ushort sfx_num)
{
	uint time = 0;
	uchar comment[256];
	uchar* p;

	snd_loads(track, file);
	snd_setvol(track, 0);
	if(loop != 0){
		snd_gettext(track, comment);
		p = get_token(comment);
		while(token_type != _NULL_){
			if(token_type == _SYMBOL_ && token[0] == '-'){
				p = get_token(p);
				if(token_type == _KEYWORD_ && strcmpi(token, "loopTime") == 0){
					p = get_token(p);
					if(token_type == _CONST_){
						time = token_value;
					}
				}
			}
			p = get_token(p);
		}
		snd_setloop(track, time);
		if(begin_time){ snd_setpos(track, begin_time); }
		apply_soundfx(track, sfx_num);
		snd_play(track, true);
	}else{
		if(begin_time){ snd_setpos(track, begin_time); }
		apply_soundfx(track, sfx_num);
		snd_play(track, false);
	}
	sound_loop_time[track] = time;
}
//===========================================================================
// ‚a‚f‚l‚ÌÄ¶‹–‰Âƒtƒ‰ƒO‚ğæ“¾
//===========================================================================
extern bool is_playable_music()
{
	return (system.master_flag && system.music_flag);
}
//===========================================================================
// ‚a‚f‚l‚Ì‰Šú‰»
//===========================================================================
extern void init_music()
{
	int i;
	for(i = 0; i < SND_BGM_MAX; i++){
		stop_music(0);
		music_play_track = (music_play_track + 1) % SND_BGM_MAX;
	}
}
//===========================================================================
// ‚a‚f‚l‚Ì’â~
//===========================================================================
extern void stop_music(uint fade_time)
{
	int track = SND_BGM + music_play_track;

	volume_fade(track, fade_volume[track], 0, fade_time);
	music_name[0] = '\0';
	music_num = 0;

	if(snd_isplay(track)){
		music_play_track = (music_play_track + 1) % SND_BGM_MAX;
	}
}
//===========================================================================
// ‚a‚f‚l‚ÌÄ¶
//===========================================================================
extern void play_music(uchar* name, uint loop, uint begin_time, uint fade_time)
{
	uchar file[256];
	int track;
	int n;

	int num = get_bgm_index(name);
	if((num >= bgm_dat_cnt) || (music_num == num)){
		return;
	}

	if(num > 0){
		strcpy(music_name, bgm_dat[num].name);
		music_loop = loop;
		music_num = num;
		if(is_playable_music()){
			track = SND_BGM + music_play_track;
			if(snd_isplay(track)){
				volume_fade(track, fade_volume[track], 0, 0);
			}
//			track = SND_BGM + music_play_track;
			sprintf(file, DIR_BGM "%s.ogg", bgm_dat[num].file);
			play_stream(track, file, loop, begin_time, music_fx);
			volume_fade(track, 0, 100, fade_time);
		}
		n = bgm_dat[num].order;
		if((n > 0) && (n < 1024)){
			system.open_bgm[n/8] |= (1 << (n % 8));
		}
	}else{
		stop_music(fade_time);
	}
}

extern void play_music_track(int num, uint begin_time, uint fade_time)
{
	uchar file[256];
	int track;
	int n;

	if((num >= bgm_dat_cnt) || (music_num == num)){
		return;
	}

	if(num > 0){
		strcpy(music_name, bgm_dat[num].name);
		music_loop = 1;
		music_num = num;
		if(is_playable_music()){
			track = SND_BGM + music_play_track;
			if(snd_isplay(track)){
				volume_fade(track, fade_volume[track], 0, 0);
			}
//			track = SND_BGM + music_play_track;
			sprintf(file, DIR_BGM "%s.ogg", bgm_dat[num].file);
			play_stream(track, file, 1, begin_time, music_fx);
			volume_fade(track, 0, 100, fade_time);
		}
		n = bgm_dat[num].order;
		if((n > 0) && (n < 1024)){
			system.open_bgm[n/8] |= (1 << (n % 8));
		}
	}else{
		stop_music(fade_time);
	}
}

extern void play_music_track_fx(int num, uint begin_time, uint fade_time, ushort sfx_num)
{
	ushort old = music_fx;
	music_fx = sfx_num;
	play_music_track(num, begin_time, fade_time);
	music_fx = old;
}
//===========================================================================
// ‚a‚f‚l‚ªÄ¶’†‚©’²‚×‚é
//===========================================================================
extern bool is_play_music()
{
	return snd_isplay(SND_BGM + music_play_track);
}
//===========================================================================
// ŠÂ‹«‰¹‚ÌÄ¶‹–‰Âƒtƒ‰ƒO‚ğæ“¾
//===========================================================================
extern bool is_playable_ambient()
{
	return (system.master_flag && system.ambient_flag);
}
//===========================================================================
// ŠÂ‹«‰¹‚Ì‰Šú‰»
//===========================================================================
extern void init_ambient()
{
	int i;
	for(i = 0; i < SND_AMB_MAX; i++){
		stop_ambient(0);
		ambient_play_track = (ambient_play_track + 1) % SND_AMB_MAX;
	}
}
//===========================================================================
// ŠÂ‹«‰¹‚Ì’â~
//===========================================================================
extern void stop_ambient(uint fade_time)
{
	int track = SND_AMB + ambient_play_track;

	volume_fade(track, fade_volume[track], 0, fade_time);
	ambient_name[0] = '\0';
	ambient_num = 0;

	if(snd_isplay(track)){
		ambient_play_track = (ambient_play_track + 1) % SND_AMB_MAX;
	}
}
//===========================================================================
// ŠÂ‹«‰¹‚ÌÄ¶
//===========================================================================
extern void play_ambient(uchar* name, uint loop, uint begin_time, uint fade_time)
{
	uchar file[256];
	int track;

	int num = get_amb_index(name);
	if((num >= amb_dat_cnt) || (ambient_num == num)){
		return;
	}

	if(num > 0){
		strcpy(ambient_name, amb_dat[num].name);
		ambient_loop = loop;
		ambient_num = num;
		if(is_playable_ambient()){
			track = SND_AMB + ambient_play_track;
			if(snd_isplay(track)){
				volume_fade(track, fade_volume[track], 0, 0);
			}
//			track = SND_AMB + ambient_play_track;
			sprintf(file, DIR_AMB "%s.ogg", amb_dat[num].file);
			play_stream(track, file, loop, begin_time, ambient_fx);
			volume_fade(track, 0, 100, fade_time);
		}
	}else{
		stop_ambient(fade_time);
	}
}

extern void play_ambient_track(int num, uint begin_time, uint fade_time)
{
	uchar file[256];
	int track;

	if((num >= amb_dat_cnt) || (ambient_num == num)){
		return;
	}

	if(num > 0){
		strcpy(ambient_name, amb_dat[num].name);
		ambient_loop = 1;
		ambient_num = num;
		if(is_playable_ambient()){
			track = SND_AMB + ambient_play_track;
			if(snd_isplay(track)){
				volume_fade(track, fade_volume[track], 0, 0);
			}
//			track = SND_AMB + ambient_play_track;
			sprintf(file, DIR_AMB "%s.ogg", amb_dat[num].file);
			play_stream(track, file, 1, begin_time, ambient_fx);
			volume_fade(track, 0, 100, fade_time);
		}
	}else{
		stop_ambient(fade_time);
	}
}

extern void play_ambient_track_fx(int num, uint begin_time, uint fade_time, ushort sfx_num)
{
	ushort old = ambient_fx;
	ambient_fx = sfx_num;
	play_ambient_track(num, begin_time, fade_time);
	ambient_fx = old;
}
//===========================================================================
// ŠÂ‹«‰¹‚ªÄ¶’†‚©’²‚×‚é
//===========================================================================
extern bool is_play_ambient()
{
	return snd_isplay(SND_AMB + ambient_play_track);
}
//===========================================================================
// ‚r‚d‚ÌÄ¶‹–‰Âƒtƒ‰ƒO‚ğæ“¾
//===========================================================================
extern bool is_playable_sound()
{
	return (system.master_flag && system.sound_flag);
}
//===========================================================================
// ‚r‚d‚Ì‰Šú‰»
//===========================================================================
extern void init_sound()
{
	stop_sound(-1, 0);
}
//===========================================================================
// ‚r‚d‚Ì’â~
//===========================================================================
extern void stop_sound(int track, uint fade_time)
{
	int i;
	if(track < 0 || track >= SND_EFE_MAX){
		for(i = 0; i < SND_EFE_MAX; i++){
			track = SND_EFE + i;
			volume_fade(track, fade_volume[track], 0, fade_time);
		}
		return;
	}
	track += SND_EFE;
	volume_fade(track, fade_volume[track], 0, fade_time);
}
//===========================================================================
// ‚r‚d‚ÌÄ¶
//===========================================================================
extern void play_sound(int track, uchar* file, uint loop, uint begin_time, uint fade_time)
{
	if(file == NULL || file[0] == '\0'){ return; }

	if(track < 0 || track >= SND_EFE_MAX){ return; }

	if(is_playable_sound()){
		if(is_play_sound(track)){
			stop_sound(track, 0);
		}
		track += SND_EFE;

		if(loop != 0){
			play_stream(track, file, loop, begin_time, sound_fx);
			volume_fade(track, 0, 100, fade_time);
			return;
		}

		if(sound_fx != 0){
			uint delay = get_soundfx_delay(sound_fx);
			snd_setmargin(delay);
			snd_load(track, file);
			snd_setmargin(0);
		}else{
			snd_load(track, file);
		}
		snd_setvol(track, 0);
		if(begin_time){ snd_setpos(track, begin_time); }
		apply_soundfx(track, sound_fx);
		snd_play(track, false);

		volume_fade(track, 0, 100, fade_time);
	}
}

extern void play_sound_track(int track, int num, int loop, uint begin_time, uint fade_time)
{
	uchar file[256];

	if(num >= se_dat_cnt){ return; }

	if(num > 0){
		sprintf(file, DIR_SE "%s.ogg", se_dat[num].file);
		play_sound(track, file, loop, begin_time, fade_time);
	}else{
		stop_sound(track, fade_time);
	}
}

extern void play_sound_track_fx(int track, int num, int loop, uint begin_time, uint fade_time, ushort sfx_num)
{
	ushort old = sound_fx;
	sound_fx = sfx_num;
	play_sound_track(track, num, loop, begin_time, fade_time);
	sound_fx = old;
}
//===========================================================================
// ‚r‚d‚ªÄ¶’†‚©’²‚×‚é
//===========================================================================
extern bool is_play_sound(int track)
{
	if(track < 0 || track >= SND_EFE_MAX){
		for(track = SND_EFE; track < SND_EFE+SND_EFE_MAX; track++){
			if(snd_isplay(track)){ return true; }
		}
		return false;
	}
	return snd_isplay(SND_EFE+track);
}
//===========================================================================
// ‰¹º‚ÌÄ¶‹–‰Âƒtƒ‰ƒO‚ğæ“¾
//===========================================================================
extern bool is_playable_voice()
{
	return (system.master_flag && system.voice_flag);
}
//===========================================================================
// ‰¹º‚Ì‰Šú‰»
//===========================================================================
extern void init_voice()
{
	stop_voice(-1, 0);
}
//===========================================================================
// ‰¹º‚Ì’â~
//===========================================================================
extern void stop_voice(int track, uint fade_time)
{
	int i;
	if(track < 0 || track >= SND_VOC_MAX){
		for(i = 0; i < SND_VOC_MAX; i++){
			voice_num[i] = 0;
			track = SND_VOC + i;
			volume_fade(track, fade_volume[track], 0, fade_time);
		}
		return;
	}
	voice_num[track] = 0;
	track += SND_VOC;
	volume_fade(track, fade_volume[track], 0, fade_time);
}
//===========================================================================
// ‰¹º‚ÌÄ¶
//===========================================================================
void play_voice_impl(int track, uchar* file, uint begin_time, uint fade_time)
{
	if(track < 0 || track >= SND_VOC_MAX){ return; }

	if(is_playable_voice()){
		track += SND_VOC;
		if(snd_isplay(track)){
			volume_fade(track, fade_volume[track], 0, fade_time);
		}
		if(voice_fx != 0){
			uint delay = get_soundfx_delay(voice_fx);
			snd_setmargin(delay);
			snd_load(track, file);
			snd_setmargin(0);
		}else{
			snd_load(track, file);
		}
		snd_setvol(track, 0);
		if(begin_time){ snd_setpos(track, begin_time); }
		apply_soundfx(track, voice_fx);
		snd_play(track, false);

		volume_fade(track, 0, 100, fade_time);
	}
}

extern void play_voice(int track, uchar* file, uint begin_time, uint fade_time)
{
	voice_num[track] = 0;
	play_voice_impl(track, file, begin_time, fade_time);
}

extern void play_voice_track(int track, int num, uint begin_time, uint fade_time)
{
	uchar file[256];
	int chr, idx;

	chr = limit(num / VOC_MASK, 0, 99);
	idx = num % VOC_MASK;
	if(idx > 0){
		stop_voice(track, 0);
		voice_num[track] = num;
		if(system.chars_flag & (1 << voice_group[chr])){
			sprintf(file, DIR_VOC "%06d.ogg", num);
			play_voice_impl(track, file, begin_time, fade_time);
		}
	}else{
		stop_voice(track, fade_time);
	}
}

extern void play_voice_track_fx(int track, int num, uint begin_time, uint fade_time, ushort sfx_num)
{
	ushort old = voice_fx;
	voice_fx = sfx_num;
	play_voice_track(track, num, begin_time, fade_time);
	voice_fx = old;
}
//===========================================================================
// ‰¹º‚ªÄ¶’†‚©’²‚×‚é
//===========================================================================
extern bool is_play_voice(int track)
{
	if(track < 0 || track >= SND_VOC_MAX){
		for(track = SND_VOC; track < SND_VOC+SND_VOC_MAX; track++){
			if(snd_isplay(track)){ return true; }
		}
		return false;
	}
	return snd_isplay(SND_VOC+track);
}
//===========================================================================
// ƒ€[ƒr[Ä¶
//===========================================================================
extern bool play_movie(uchar* file, bool skip)
{
	int vol = (inner_music_vol * system.music_vol * system.master_vol) / 10000;
	media_open(file);
	media_setvol(is_playable_music() ? vol : 0);
	media_size(RC(0,0,800,600));
	media_play();
	bool result = true;
	while(media_isplay()){
		input_key();
		if(skip && (push_key & (KEY_DECIDE|KEY_CANCEL))){
			result = false;
//			media_stop();
			break;
		}
	}
	media_close();
	return result;
}
//===========================================================================
// Œø‰Ê‰¹‚ÌÄ¶(ƒVƒXƒeƒ€AƒGƒtƒFƒNƒg—p)
//===========================================================================
uint se_play_frame[SND_FRE_MAX];
uchar se_play_file[SND_FRE_MAX][64];
int se_play_next;

extern int play_frame_sound_imp(uchar* file, uint fade_time, bool loop)
{
	uchar buf[256];
	int track;
	int i;

	if(!is_playable_sound() || (file == NULL)){
		return -1;
	}

	for(i = 0; i < SND_FRE_MAX; i++){
		if((se_play_frame[i] == task_time) && (strcmp(se_play_file[i], file) == 0)){
			return i;
		}
	}
	for(i = 0; i < SND_FRE_MAX; i++){
		if(!snd_isplay(SND_FRE+se_play_next)){ break; }
		se_play_next = (se_play_next + 1) % SND_FRE_MAX;
	}
//	if(i >= SND_FRE_MAX){ return; }

	track = SND_FRE + se_play_next;
	snd_setvol(track, 0);
	snd_free(track);

	sprintf(buf, DIR_SE "%s.ogg", file);
	snd_load(track, buf);
	snd_setvol(track, 0);
	snd_play(track, loop);

	volume_fade(track, 0, 100, fade_time);

	se_play_frame[se_play_next] = task_time;
	strcpy(se_play_file[se_play_next], file);

	se_play_next = (se_play_next + 1) % SND_FRE_MAX;

	return track - SND_FRE;
}

extern int play_frame_sound(uchar* file, uint fade_time)
{
	return play_frame_sound_imp(file, fade_time, false);
}

extern int play_frame_sound_num(int num, uint fade_time)
{
	uchar buf[256];
	sprintf(buf, "eff%03d", num);
	return play_frame_sound(buf, fade_time);
}
//===========================================================================
// Œø‰Ê‰¹‚Ì’â~(ƒVƒXƒeƒ€AƒGƒtƒFƒNƒg—p)
//===========================================================================
extern void stop_frame_sound(int track, uint fade_time)
{
	int i;
	if(track < 0 || track >= SND_FRE_MAX){
		for(i = 0; i < SND_FRE_MAX; i++){
			track = SND_FRE + i;
			volume_fade(track, fade_volume[track], 0, fade_time);
		}
		return;
	}
	track += SND_FRE;
	volume_fade(track, fade_volume[track], 0, fade_time);
}
//===========================================================================
// Œø‰Ê‰¹‚Ìƒpƒ“İ’è(ƒVƒXƒeƒ€AƒGƒtƒFƒNƒg—p)
//===========================================================================
extern void pan_frame_sound(int track, int pan)
{
	if(track >= 0 && track < SND_FRE_MAX){
		track += SND_FRE;
		snd_setpan(track, pan);
	}
}
//===========================================================================
// ƒVƒXƒeƒ€‚r‚d‚Ìƒ[ƒh
//===========================================================================
extern void load_system_se(int track, uchar* name)
{
	uchar buf[256];
	int index;

	free_system_se(track);

	if(track >= 0 && track < SND_SYS_MAX){
		track += SND_SYS;
		index = get_se_index(name);
		if(index > 0){
			sprintf(buf, DIR_SE "%s.ogg", se_dat[index].file);
			snd_load(track, buf);
			snd_setvol(track, get_volume(track));
		}
	}
}
//===========================================================================
// ƒVƒXƒeƒ€‚r‚d‚Ì‰ğ•ú
//===========================================================================
extern void free_system_se(int track)
{
	if(track >= 0 && track < SND_SYS_MAX){
		track += SND_SYS;
		snd_free(track);
	}
}
//===========================================================================
// ƒVƒXƒeƒ€‚r‚d‚ÌÄ¶
//===========================================================================
extern void play_system_se(int track)
{
	if(track >= 0 && track < SND_SYS_MAX){
		track += SND_SYS;
		if(is_playable_sound()){
			snd_play(track, false);
		}
	}
}
//===========================================================================
// ƒVƒXƒeƒ€‚r‚d‚Ì’â~
//===========================================================================
extern void stop_system_se(int track)
{
	if(track >= 0 && track < SND_SYS_MAX){
		track += SND_SYS;
		snd_stop(track);
	}
}
//===========================================================================
uint effect_time = 1000;
uint effect_trans = 0;

void backscreen_copy()
{
	apage(BACKSCREEN);
	sp_redraw(disp_flag);
	sp_draw(disp_flag, NULL);
	apage(SCREEN);
}
//===========================================================================
// ƒƒCƒvˆ—
//===========================================================================
OBJ( effect_wipe )
{
	static int page;
	static int alpha;
	uint tick;
	uchar buf[256];
	int i, n;

	switch( task->phase ){
	case 0:
		task_all_pause(TP_NORMAL_MASK);
		task_resume(task);
		backscreen_copy();
		galloc(&page, SCREEN_W, SCREEN_H, 24);
		gcopy(page, 0, 0, SCREEN, NULL, NULL);
		sprintf(buf, DIR_SYS "wipe%02d.bmp", task->data);
		gload(&alpha, buf);
		gattach(BACKSCREEN, alpha);
		task->data = 0;
		task->phase++;
		break;
	case 1:
		if(task->data == 0){
			reset_tick(task);
			task->data = 1;
		}
		tick = get_tick(task);
		if(!(push_key & KEY_DECIDE) && (tick < effect_time)){
			n = limit(itrans(effect_trans, 0, 255, (float)tick/effect_time), 0, 255);
			gcopy(SCREEN, 0, 0, page, NULL, NULL);
			gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, PATBLEND(n, 32));
			update_rect(0, 0, SCREEN_W, SCREEN_H);
			break;
		}
		gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, NULL);
		update_rect(0, 0, SCREEN_W, SCREEN_H);
		task->phase = -1;
		break;
	case -1:
		gdetach(BACKSCREEN);
		gfree(alpha);
		gfree(page);
		task_all_resume(TP_NORMAL_MASK);
		task_end(task);
		break;
	}
}
//===========================================================================
// ƒuƒŒƒ“ƒh
//===========================================================================
OBJ( effect_blend )
{
	static int page;
	uint tick;
	int i, n;

	switch( task->phase ){
	case 0:
		task_all_pause(TP_NORMAL_MASK);
		task_resume(task);
		backscreen_copy();
		galloc(&page, SCREEN_W, SCREEN_H, 24);
		gcopy(page, 0, 0, SCREEN, NULL, NULL);
		task->data = 0;
		task->phase++;
		break;
	case 1:
		if(task->data == 0){
			reset_tick(task);
			task->data = 1;
		}
		tick = get_tick(task);
		if(!(push_key & KEY_DECIDE) && (tick < effect_time)){
			n = limit(itrans(effect_trans, 0, 255, (float)tick/effect_time), 0, 255);
			gcopy(SCREEN, 0, 0, page, NULL, NULL);
			gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, BLEND(n));
			update_rect(0, 0, SCREEN_W, SCREEN_H);
			break;
		}
		gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, NULL);
		update_rect(0, 0, SCREEN_W, SCREEN_H);
		task->phase = -1;
		break;
	case -1:
		gfree(page);
		task_all_resume(TP_NORMAL_MASK);
		task_end(task);
		break;
	}
}
//===========================================================================
// ƒ‚ƒUƒCƒN
//===========================================================================
OBJ( effect_mosaic )
{
	static uint level;
	static int page;
	uint tick;
	int i, n;

	switch( task->phase ){
	case 0:
		task_all_pause(TP_NORMAL_MASK);
		task_resume(task);
		backscreen_copy();
		galloc(&page, SCREEN_W, SCREEN_H, 24);
		gcopy(page, 0, 0, SCREEN, NULL, NULL);
		task->data = 0;
		task->phase++;
		break;
	case 1:
		if(task->data == 0){
			level = 0;
			reset_tick(task);
			task->data = 1;
		}
		tick = get_tick(task);
		if(!(push_key & KEY_DECIDE) && (tick < effect_time)){
			i = (tick * 255) / effect_time;
			if((i - level) >= 8){
				if(i <= 0 || i >= 255){
					n = i;
				}else{
					n = ((i - level) * 65280) / (255 - i);
					n = ((n * 255) / (255 + (n >> 8))) >> 8;
				}
				gcopy(page, 0, 0, BACKSCREEN, NULL, BLEND(n));
				level = i;
			}
			n = 40 - abs(limit(itrans(effect_trans, 0, 80, (float)tick/effect_time), 0, 80) - 40);
			mosaic(SCREEN, 0, 0, page, NULL, n, n);
			update_rect(0, 0, SCREEN_W, SCREEN_H);
			break;
		}
		gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, NULL);
		update_rect(0, 0, SCREEN_W, SCREEN_H);
		task->phase = -1;
		break;
	case -1:
		gfree(page);
		task_all_resume(TP_NORMAL_MASK);
		task_end(task);
		break;
	}
}
//===========================================================================
// ƒ^ƒCƒ‹
//===========================================================================
OBJ( effect_tile )
{
	static uint delay[8*6];
	static int base;

	uint tick;
	POINT pt[4];
	int count;
	int rot;
	int x, y;
	int i, j, n;

	switch( task->phase ){
	case 0:
		task_all_pause(TP_NORMAL_MASK);
		task_resume(task);
		backscreen_copy();
		galloc(&base, SCREEN_W, SCREEN_H, 24);
		gcopy(base, 0, 0, SCREEN, NULL, NULL);
		for(i = 0; i < 8*6; i++)
			delay[i] = (i / 8) * 150 + (i % 8) * 40 + 1;
		task->data = 0;
		task->phase++;
		break;
	case 1:
		if(task->data == 0){
			reset_tick(task);
			task->data = 1;
		}
		tick = get_tick(task);
		if(!(push_key & KEY_DECIDE)/* && (tick < effect_time)*/){
			count = 0;
			for(i = 0; i < 8*6; i++){
				if(delay[i] == 0){ continue; }
				if(tick > delay[i]){
					n = tick - delay[i];
					if(n >= effect_time){
						n = effect_time;
						delay[i] = 0;
					}
					x = (i % 8) * 100;
					y = (i / 8) * 100;
					rot = limit(itrans(effect_trans, 0, 1024, (float)n/effect_time), 0, 1024);
					j = (_sin(rot) * 50) >> 16;

					gfill(SCREEN, RC(x,y,100,100), 0);
					if(j != 50) {
						pt[0].x = x+j;    pt[0].y = y+j;
						pt[1].x = x;      pt[1].y = y+99;
						pt[2].x = x+99-j; pt[2].y = y+99-j;
						pt[3].x = x+99;   pt[3].y = y;
						n = (rot < 512) ? base : BACKSCREEN;
						freemap(SCREEN, pt, n, RC(x,y,100,100), -1);
					}
					update_rect(0, 0, SCREEN_W, SCREEN_H);
				}
				count++;
			}
			if(count > 0){ break; }
		}
		gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, NULL);
		update_rect(0, 0, SCREEN_W, SCREEN_H);
		task->phase = -1;
		break;
	case -1:
		gfree(base);
		task_all_resume(TP_NORMAL_MASK);
		task_end(task);
		break;
	}
}
//===========================================================================
// ƒmƒCƒY
//===========================================================================
OBJ( effect_noise )
{
	static int page;
	uint tick;
	int i, n;

	switch( task->phase ){
	case 0:
		task_all_pause(TP_NORMAL_MASK);
		task_resume(task);
		backscreen_copy();
		galloc(&page, SCREEN_W, SCREEN_H, 24);
		gcopy(page, 0, 0, SCREEN, NULL, NULL);
		task->data = 0;
		task->phase++;
		break;
	case 1:
		if(task->data == 0){
			reset_tick(task);
			task->data = 1;
		}
		tick = get_tick(task);
		if(!(push_key & KEY_DECIDE) && (tick < effect_time)){
			n = limit(itrans(effect_trans, 0, 255, (float)tick/effect_time), 0, 255);
			gcopy(SCREEN, 0, 0, page, NULL, NULL);
			gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, BLEND(n));
			i = limit(itrans(effect_trans, 0, 1024, (float)tick/effect_time), 0, 1024);
			noise(SCREEN, NULL, _sin(i) >> 9);
			update_rect(0, 0, SCREEN_W, SCREEN_H);
			break;
		}
		gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, NULL);
		update_rect(0, 0, SCREEN_W, SCREEN_H);
		task->phase = -1;
		break;
	case -1:
		gfree(page);
		task_all_resume(TP_NORMAL_MASK);
		task_end(task);
		break;
	}
}
//===========================================================================
// ƒXƒ‰ƒCƒX
//===========================================================================
OBJ( effect_slice )
{
	static int page;
	uint tick;
	int i, n;

	switch( task->phase ){
	case 0:
		task_all_pause(TP_NORMAL_MASK);
		task_resume(task);
		backscreen_copy();
		galloc(&page, SCREEN_W, SCREEN_H, 24);
		gcopy(page, 0, 0, SCREEN, NULL, NULL);
		task->data = 0;
		task->phase++;
		break;
	case 1:
		if(task->data == 0){
			reset_tick(task);
			task->data = 1;
		}
		tick = get_tick(task);
		if(!(push_key & KEY_DECIDE) && (tick < effect_time)){
			n = limit(itrans(effect_trans, 0, 255, (float)tick/effect_time), 0, 255);
			gcopy(SCREEN, 0, 0, page, NULL, NULL);
			gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, BLEND(n));
			i = 128 - abs(limit(itrans(effect_trans, 0, 256, (float)tick/effect_time), 0, 256) - 128);
			slice(SCREEN, NULL, i);
			update_rect(0, 0, SCREEN_W, SCREEN_H);
			break;
		}
		gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, NULL);
		update_rect(0, 0, SCREEN_W, SCREEN_H);
		task->phase = -1;
		break;
	case -1:
		gfree(page);
		task_all_resume(TP_NORMAL_MASK);
		task_end(task);
		break;
	}
}
//===========================================================================
// ƒuƒ‰[
//===========================================================================
OBJ( effect_blur )
{
	static uint level;
	static int page;
	uint tick;
	int i, n;

	switch( task->phase ){
	case 0:
		task_all_pause(TP_NORMAL_MASK);
		task_resume(task);
		backscreen_copy();
		galloc(&page, SCREEN_W, SCREEN_H, 24);
		gcopy(page, 0, 0, SCREEN, NULL, NULL);
		task->data = 0;
		task->phase++;
		break;
	case 1:
		if(task->data == 0){
			level = 0;
			reset_tick(task);
			task->data = 1;
		}
		tick = get_tick(task);
		if(!(push_key & KEY_DECIDE) && (tick < effect_time)){
			i = (tick * 255) / effect_time;
			if((i - level) >= 8){
				if(i <= 0 || i >= 255){
					n = i;
				}else{
					n = ((i - level) * 65280) / (255 - i);
					n = ((n * 255) / (255 + (n >> 8))) >> 8;
				}
				gcopy(page, 0, 0, BACKSCREEN, NULL, BLEND(n));
				level = i;
			}
			n = 128 - abs(limit(itrans(effect_trans, 0, 256, (float)tick/effect_time), 0, 256) - 128);
			blur(SCREEN, 0, 0, page, NULL, n);
			update_rect(0, 0, SCREEN_W, SCREEN_H);
			break;
		}
		gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, NULL);
		update_rect(0, 0, SCREEN_W, SCREEN_H);
		task->phase = -1;
		break;
	case -1:
		gfree(page);
		task_all_resume(TP_NORMAL_MASK);
		task_end(task);
		break;
	}
}
//===========================================================================
// ƒ‰ƒXƒ^[
//===========================================================================
OBJ( effect_raster )
{
	static uint level;
	static int page;
	uint tick;
	int i, n;

	switch( task->phase ){
	case 0:
		task_all_pause(TP_NORMAL_MASK);
		task_resume(task);
		backscreen_copy();
		galloc(&page, SCREEN_W, SCREEN_H, 24);
		gcopy(page, 0, 0, SCREEN, NULL, NULL);
		ef_init(0, SCREEN, NULL, page, NULL);
		task->data = 0;
		task->phase++;
		break;
	case 1:
		if(task->data == 0){
			level = 0;
			reset_tick(task);
			task->data = 1;
		}
		tick = get_tick(task);
		if(!(push_key & KEY_DECIDE) && (tick < effect_time)){
			i = (tick * 255) / effect_time;
			if((i - level) >= 8){
				if(i <= 0 || i >= 255){
					n = i;
				}else{
					n = ((i - level) * 65280) / (255 - i);
					n = ((n * 255) / (255 + (n >> 8))) >> 8;
				}
				gcopy(page, 0, 0, BACKSCREEN, NULL, BLEND(n));
				level = i;
			}
			n = 200 - abs(limit(itrans(effect_trans, 0, 400, (float)tick/effect_time), 0, 400) - 200);
			ef_raster(0, (tick / 10), 100, n);
			update_rect(0, 0, SCREEN_W, SCREEN_H);
			break;
		}
		gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, NULL);
		update_rect(0, 0, SCREEN_W, SCREEN_H);
		task->phase = -1;
		break;
	case -1:
		ef_free(0);
		gfree(page);
		task_all_resume(TP_NORMAL_MASK);
		task_end(task);
		break;
	}
}
//===========================================================================
// ‰ñ“]Šg‘å
//===========================================================================
OBJ( effect_rot_zoom_in )
{
	uint tick;
	int rot;
	int x, y, w, h;
	int i, n;
	float f;

	switch( task->phase ){
	case 0:
		task_all_pause(TP_NORMAL_MASK);
		task_resume(task);
		backscreen_copy();
		task->data = 0;
		task->phase++;
		break;
	case 1:
		if(task->data == 0){
			reset_tick(task);
			task->data = 1;
		}
		tick = get_tick(task);
		if(!(push_key & KEY_DECIDE) && (tick < effect_time)){
			f = (float)tick / effect_time;
			n = limit(itrans(effect_trans, 0, 255, f), 0, 255);
			w = itrans(effect_trans, 0, 800, f);
			h = itrans(effect_trans, 0, 600, f);
			w = (w * (65536 + _cos(itrans(effect_trans, 1536, 0, f)))) >> 17;
			h = (h * (65536 + _cos(itrans(effect_trans, 768, 0, f)))) >> 17;
			x = (800 - w) / 2;
			y = (600 - h) / 2;
			rot = itrans(effect_trans, 1024, 0, f);
			gblit(SCREEN, RCD(x,y,w,h), BACKSCREEN, RCS(0,0,800,600), BLEND(n), rot);
			update_rect(0, 0, SCREEN_W, SCREEN_H);
			break;
		}
		gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, NULL);
		update_rect(0, 0, SCREEN_W, SCREEN_H);
		task->phase = -1;
		break;
	case -1:
		task_all_resume(TP_NORMAL_MASK);
		task_end(task);
		break;
	}
}
//===========================================================================
// ‰ñ“]k¬
//===========================================================================
OBJ( effect_rot_zoom_out )
{
	static int page;
	uint tick;
	int rot;
	int x, y, w, h;
	int i, n;
	float f;

	switch( task->phase ){
	case 0:
		task_all_pause(TP_NORMAL_MASK);
		task_resume(task);
		backscreen_copy();
		galloc(&page, SCREEN_W, SCREEN_H, 24);
		gcopy(page, 0, 0, SCREEN, NULL, NULL);
		task->data = 0;
		task->phase++;
		break;
	case 1:
		if(task->data == 0){
			reset_tick(task);
			task->data = 1;
		}
		tick = get_tick(task);
		if(!(push_key & KEY_DECIDE) && (tick < effect_time)) {
			f = (float)tick / effect_time;
			n = limit(itrans(effect_trans, 0, 255, f), 0, 255);
			gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, BLEND(n));
			w = itrans(effect_trans, 800, 0, f);
			h = itrans(effect_trans, 600, 0, f);
			w = (w * (65536 + _cos(itrans(effect_trans, 0, 1536, f)))) >> 17;
			h = (h * (65536 + _cos(itrans(effect_trans, 0, 768, f)))) >> 17;
			x = (800 - w) / 2;
			y = (600 - h) / 2;
			rot = itrans(effect_trans, 0, -1024, f);
			gblit(SCREEN, RCD(x,y,w,h), page, RCS(0,0,800,600), BLEND(255-n), rot);
			update_rect(0, 0, SCREEN_W, SCREEN_H);
			break;
		}
		gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, NULL);
		update_rect(0, 0, SCREEN_W, SCREEN_H);
		task->phase = -1;
		break;
	case -1:
		gfree(page);
		task_all_resume(TP_NORMAL_MASK);
		task_end(task);
		break;
	}
}
//===========================================================================
// ‰QŠª‚«
//===========================================================================
OBJ( effect_whirl )
{
	static int page;
	uint tick;
	int amp;
	int n;
	float f;

	switch( task->phase ){
	case 0:
		task_all_pause(TP_NORMAL_MASK);
		task_resume(task);
		backscreen_copy();
		galloc(&page, SCREEN_W, SCREEN_H, 24);
		gcopy(page, 0, 0, SCREEN, NULL, NULL);
		ef_init(0, SCREEN, NULL, page, NULL);
		task->data = 0;
		task->phase++;
		break;
	case 1:
		if(task->data == 0){
			reset_tick(task);
			task->data = 1;
		}
		tick = get_tick(task);
		if(!(push_key & KEY_DECIDE) && (tick < effect_time)){
			f = (float)tick / effect_time;
			amp = itrans(effect_trans, 0, -100, f);
			ef_whirl(0, amp);
			n = limit(itrans(effect_trans, 0, 255, f), 0, 255);
			gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, BLEND(n));
			update_rect(0, 0, SCREEN_W, SCREEN_H);
			break;
		}
		gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, NULL);
		update_rect(0, 0, SCREEN_W, SCREEN_H);
		task->phase = -1;
		break;
	case -1:
		ef_free(0);
		gfree(page);
		task_all_resume(TP_NORMAL_MASK);
		task_end(task);
		break;
	}
}
//===========================================================================
// Šg‘å
//===========================================================================
OBJ( effect_zoom_in )
{
	static int page;
	uint tick;
	int rot, zoom;
	int n;
	float f;

	switch( task->phase ){
	case 0:
		task_all_pause(TP_NORMAL_MASK);
		task_resume(task);
		backscreen_copy();
		galloc(&page, SCREEN_W, SCREEN_H, 24);
		gcopy(page, 0, 0, SCREEN, NULL, NULL);
		ef_init(0, SCREEN, NULL, page, NULL);
		task->data = 0;
		task->phase++;
		break;
	case 1:
		if(task->data == 0){
			reset_tick(task);
			task->data = 1;
		}
		tick = get_tick(task);
		if(!(push_key & KEY_DECIDE) && (tick < effect_time)){
			f = (float)tick / effect_time;
			rot = itrans(effect_trans, 0, -256, f);
			zoom = itrans(effect_trans, FL(1), FL(0), f);
			ef_rotate(0, 400, 300, zoom, rot);
			n = limit(itrans(effect_trans, 0, 255, f), 0, 255);
			gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, BLEND(n));
			update_rect(0, 0, SCREEN_W, SCREEN_H);
			break;
		}
		gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, NULL);
		update_rect(0, 0, SCREEN_W, SCREEN_H);
		task->phase = -1;
		break;
	case -1:
		ef_free(0);
		gfree(page);
		task_all_resume(TP_NORMAL_MASK);
		task_end(task);
		break;
	}
}
//===========================================================================
// k¬
//===========================================================================
OBJ( effect_zoom_out )
{
	static int page;
	uint tick;
	int rot, zoom;
	int n;
	float f;

	switch( task->phase ){
	case 0:
		task_all_pause(TP_NORMAL_MASK);
		task_resume(task);
		backscreen_copy();
		galloc(&page, SCREEN_W, SCREEN_H, 24);
		gcopy(page, 0, 0, SCREEN, NULL, NULL);
		ef_init(0, SCREEN, NULL, page, NULL);
		task->data = 0;
		task->phase++;
		break;
	case 1:
		if(task->data == 0){
			reset_tick(task);
			task->data = 1;
		}
		tick = get_tick(task);
		if(!(push_key & KEY_DECIDE) && (tick < effect_time)){
			f = (float)tick / effect_time;
			rot = itrans(effect_trans, 0, -256, f);
			zoom = itrans(effect_trans, FL(1), FL(2), f);
			ef_rotate(0, 400, 300, zoom, rot);
			n = limit(itrans(effect_trans, 0, 255, f), 0, 255);
			gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, BLEND(n));
			update_rect(0, 0, SCREEN_W, SCREEN_H);
			break;
		}
		gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, NULL);
		update_rect(0, 0, SCREEN_W, SCREEN_H);
		task->phase = -1;
		break;
	case -1:
		ef_free(0);
		gfree(page);
		task_all_resume(TP_NORMAL_MASK);
		task_end(task);
		break;
	}
}
//===========================================================================
// ‰¡‰ñ“]
//===========================================================================
OBJ( effect_rot_axis_y )
{
	static int page;
	uint tick;
	POINT pt[4];
	int x, y;
	int n;

	switch( task->phase ){
	case 0:
		task_all_pause(TP_NORMAL_MASK);
		task_resume(task);
		backscreen_copy();
		galloc(&page, SCREEN_W, SCREEN_H, 24);
		gcopy(page, 0, 0, SCREEN, NULL, NULL);
		task->data = 0;
		task->phase++;
		break;
	case 1:
		if(task->data == 0){
			reset_tick(task);
			task->data = 1;
		}
		tick = get_tick(task);
		if(!(push_key & KEY_DECIDE) && (tick < effect_time)){
			n = itrans(effect_trans, 0, 1024, (float)tick/effect_time);
			gfill(SCREEN, NULL, 0);
			x = FR(_cos(n) * 400);
			y = FR(_sin(n) * 100);
			if(n < 512){
				pt[0].x = 400 - x;     pt[0].y = 0 - y;
				pt[1].x = 400 - x;     pt[1].y = 600 + y - 1;
				pt[2].x = 400 + x - 1; pt[2].y = 600 - y - 1;
				pt[3].x = 400 + x - 1; pt[3].y = 0 + y;
				freemap(SCREEN, pt, page, RC(0,0,SCREEN_W,SCREEN_H), -1);
			}else{
				pt[0].x = 400 + x;     pt[0].y = 0 + y;
				pt[1].x = 400 + x;     pt[1].y = 600 - y - 1;
				pt[2].x = 400 - x - 1; pt[2].y = 600 + y - 1;
				pt[3].x = 400 - x - 1; pt[3].y = 0 - y;
				freemap(SCREEN, pt, BACKSCREEN, RC(0,0,SCREEN_W,SCREEN_H), -1);
			}
			update_rect(0, 0, SCREEN_W, SCREEN_H);
			break;
		}
		gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, NULL);
		update_rect(0, 0, SCREEN_W, SCREEN_H);
		task->phase = -1;
		break;
	case -1:
		gfree(page);
		task_all_resume(TP_NORMAL_MASK);
		task_end(task);
		break;
	}
}
//===========================================================================
// c‰ñ“]
//===========================================================================
OBJ( effect_rot_axis_x )
{
	static int page;
	uint tick;
	POINT pt[4];
	int x, y;
	int n;

	switch( task->phase ){
	case 0:
		task_all_pause(TP_NORMAL_MASK);
		task_resume(task);
		backscreen_copy();
		galloc(&page, SCREEN_W, SCREEN_H, 24);
		gcopy(page, 0, 0, SCREEN, NULL, NULL);
		task->data = 0;
		task->phase++;
		break;
	case 1:
		if(task->data == 0){
			reset_tick(task);
			task->data = 1;
		}
		tick = get_tick(task);
		if(!(push_key & KEY_DECIDE) && (tick < effect_time)){
			n = itrans(effect_trans, 0, 1024, (float)tick/effect_time);
			gfill(SCREEN, NULL, 0);
			x = FR(_sin(n) * 100);
			y = FR(_cos(n) * 300);
			if(n < 512){
				pt[0].x = 0 - x;       pt[0].y = 300 - y;
				pt[1].x = 0 + x;       pt[1].y = 300 + y - 1;
				pt[2].x = 800 - x - 1; pt[2].y = 300 + y - 1;
				pt[3].x = 800 + x - 1; pt[3].y = 300 - y;
				freemap(SCREEN, pt, page, RC(0,0,SCREEN_W,SCREEN_H), -1);
			}else{
				pt[0].x = 0 + x;       pt[0].y = 300 + y;
				pt[1].x = 0 - x;       pt[1].y = 300 - y - 1;
				pt[2].x = 800 + x - 1; pt[2].y = 300 - y - 1;
				pt[3].x = 800 - x - 1; pt[3].y = 300 + y;
				freemap(SCREEN, pt, BACKSCREEN, RC(0,0,SCREEN_W,SCREEN_H), -1);
			}
			update_rect(0, 0, SCREEN_W, SCREEN_H);
			break;
		}
		gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, NULL);
		update_rect(0, 0, SCREEN_W, SCREEN_H);
		task->phase = -1;
		break;
	case -1:
		gfree(page);
		task_all_resume(TP_NORMAL_MASK);
		task_end(task);
		break;
	}
}
//===========================================================================
// Ÿø‚İ
//===========================================================================
OBJ( effect_blot )
{
	static int page;
	uint tick;
	int  n;

	switch( task->phase ){
	case 0:
		task_all_pause(TP_NORMAL_MASK);
		task_resume(task);
		backscreen_copy();
		galloc(&page, SCREEN_W, SCREEN_H, 24);
		gcopy(page, 0, 0, SCREEN, NULL, NULL);
		task->data = 0;
		task->phase++;
		break;
	case 1:
		if(task->data == 0){
			reset_tick(task);
			task->data = 1;
		}
		tick = get_tick(task);
		if(!(push_key & KEY_DECIDE) && (tick < effect_time)){
			n = limit(itrans(effect_trans, 0, 255, (float)tick/effect_time), 0, 255);
			fire(page, NULL);
//			gradation(page, 0, 0, page, NULL);
			gcopy(SCREEN, 0, 0, page, NULL, NULL);
			gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, BLEND(n));
			update_rect(0, 0, SCREEN_W, SCREEN_H);
			break;
		}
		gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, NULL);
		update_rect(0, 0, SCREEN_W, SCREEN_H);
		task->phase = -1;
		break;
	case -1:
		gfree(page);
		task_all_resume(TP_NORMAL_MASK);
		task_end(task);
		break;
	}
}
//===========================================================================
// ‰EƒXƒ‰ƒCƒh
//===========================================================================
OBJ( effect_slide_r )
{
	static int dir;
	static int page;
	uint tick;
	int i, n;

	switch( task->phase ){
	case 0:
		task_all_pause(TP_NORMAL_MASK);
		task_resume(task);
		backscreen_copy();
		galloc(&page, SCREEN_W, SCREEN_H, 24);
		gcopy(page, 0, 0, SCREEN, NULL, NULL);
		dir = task->data;
		task->data = 0;
		task->phase++;
		break;
	case 1:
		if(task->data == 0){
			reset_tick(task);
			task->data = 1;
		}
		tick = get_tick(task);
		if(!(push_key & KEY_DECIDE) && (tick < effect_time)){
			i = itrans(effect_trans, 0, 800, (float)tick/effect_time);
			n = limit(itrans(effect_trans, 0, 255, (float)tick/effect_time), 0, 255);
			if(dir == 0){
				gcopy(SCREEN, 0, 0, page, NULL, NULL);
				gcopy(SCREEN, i-800, 0, BACKSCREEN, NULL, BLEND(n));
			}else{
				gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, NULL);
				gcopy(SCREEN, i, 0, page, NULL, BLEND(255-n));
			}
			update_rect(0, 0, SCREEN_W, SCREEN_H);
			break;
		}
		gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, NULL);
		update_rect(0, 0, SCREEN_W, SCREEN_H);
		task->phase = -1;
		break;
	case -1:
		gfree(page);
		task_all_resume(TP_NORMAL_MASK);
		task_end(task);
		break;
	}
}
//===========================================================================
// ¶ƒXƒ‰ƒCƒh
//===========================================================================
OBJ( effect_slide_l )
{
	static int dir;
	static int page;
	uint tick;
	int i, n;

	switch( task->phase ){
	case 0:
		task_all_pause(TP_NORMAL_MASK);
		task_resume(task);
		backscreen_copy();
		galloc(&page, SCREEN_W, SCREEN_H, 24);
		gcopy(page, 0, 0, SCREEN, NULL, NULL);
		dir = task->data;
		task->data = 0;
		task->phase++;
		break;
	case 1:
		if(task->data == 0){
			reset_tick(task);
			task->data = 1;
		}
		tick = get_tick(task);
		if(!(push_key & KEY_DECIDE) && (tick < effect_time)){
			i = itrans(effect_trans, 0, 800, (float)tick/effect_time);
			n = limit(itrans(effect_trans, 0, 255, (float)tick/effect_time), 0, 255);
			if(dir == 0){
				gcopy(SCREEN, 0, 0, page, NULL, NULL);
				gcopy(SCREEN, 800-i, 0, BACKSCREEN, NULL, BLEND(n));
			}else{
				gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, NULL);
				gcopy(SCREEN, -i, 0, page, NULL, BLEND(255-n));
			}
			update_rect(0, 0, SCREEN_W, SCREEN_H);
			break;
		}
		gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, NULL);
		update_rect(0, 0, SCREEN_W, SCREEN_H);
		task->phase = -1;
		break;
	case -1:
		gfree(page);
		task_all_resume(TP_NORMAL_MASK);
		task_end(task);
		break;
	}
}
//===========================================================================
// ¶‰E•ªŠ„
//===========================================================================
OBJ( effect_split )
{
	static int dir;
	static int page;
	uint tick;
	int i, n;

	switch( task->phase ){
	case 0:
		task_all_pause(TP_NORMAL_MASK);
		task_resume(task);
		backscreen_copy();
		galloc(&page, SCREEN_W, SCREEN_H, 24);
		gcopy(page, 0, 0, SCREEN, NULL, NULL);
		dir = task->data;
		task->data = 0;
		task->phase++;
		break;
	case 1:
		if(task->data == 0){
			reset_tick(task);
			task->data = 1;
		}
		tick = get_tick(task);
		if(!(push_key & KEY_DECIDE) && (tick < effect_time)){
			i = limit(itrans(effect_trans, 0, 400, (float)tick/effect_time), 0, 400);
			n = limit(itrans(effect_trans, 0, 255, (float)tick/effect_time), 0, 255);
			if(dir == 0){
				gcopy(SCREEN, 0, 0, page, NULL, NULL);
				gcopy(SCREEN, -400+i, 0, BACKSCREEN, RC(0,0,400,600), BLEND(n));
				gcopy(SCREEN, 800-i, 0, BACKSCREEN, RC(400,0,400,600), BLEND(n));
			}else{
				gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, NULL);
				gcopy(SCREEN, -i, 0, page, RC(0,0,400,600), BLEND(255-n));
				gcopy(SCREEN, 400+i, 0, page, RC(400,0,400,600), BLEND(255-n));
			}
			update_rect(0, 0, SCREEN_W, SCREEN_H);
			break;
		}
		gcopy(SCREEN, 0, 0, BACKSCREEN, NULL, NULL);
		update_rect(0, 0, SCREEN_W, SCREEN_H);
		task->phase = -1;
		break;
	case -1:
		gfree(page);
		task_all_resume(TP_NORMAL_MASK);
		task_end(task);
		break;
	}
}
//===========================================================================
// ‰æ–ÊØ‚è‘Ö‚¦ƒGƒtƒFƒNƒg
//===========================================================================
void (*effect_list[22][3])() = {
	effect_wipe, 1000, 0,
	effect_blend, 1000, 0,
	effect_mosaic, 1000, 0,
	effect_tile, 2000, 0,
	effect_noise, 2000, 0,
	effect_slice, 2000, 0,
	effect_blur, 2000, 0,
	effect_raster, 2000, 0,
	effect_rot_zoom_in, 2000, 0,
	effect_rot_zoom_out, 2000, 0,
	effect_whirl, 2000, 0,
	effect_zoom_in, 2000, 0,
	effect_zoom_out, 2000, 0,
	effect_rot_axis_y, 2000, 0,
	effect_rot_axis_x, 2000, 0,
	effect_blot, 2000, 0,
	effect_slide_r, 800, 0,
	effect_slide_r, 800, 1,
	effect_slide_l, 800, 0,
	effect_slide_l, 800, 1,
	effect_split, 800, 0,
	effect_split, 800, 1
};


extern void cross_fade_ex(int pattern, uint time, int curve)
{
	int param;

	if((pattern != 0) && (pattern > -22)){
		if(pattern < 0){
			pattern = -pattern;
			param = effect_list[pattern][2];
		}else{
			param = pattern;
			pattern = 0;
		}
		effect_trans = curve;
		effect_time = time > 0 ? time : effect_list[pattern][1];
		task_begin(3, effect_list[pattern][0], param, 0, -1);
	}
}
extern void cross_fade(int pattern, uint time)
{
	cross_fade_ex(pattern, time, 0);
}
//===========================================================================
// Šm”FƒƒbƒZ[ƒW‚Ì•\¦
//===========================================================================
int bt_confirm_bg;
int bt_confirm_frame;
int bt_confirm_text;

SPRITE* sp_confirm_bg;
SPRITE* sp_confirm_frame;
SPRITE* sp_confirm_message;
SPRITE* sp_confirm_button[2];
SPRITE* sp_confirm_answer[2];

EVT( ev_confirm_button )
{
	int n;

	switch( msg ){
	case EV_INIT:
		ev->keyboard = true;
		break;
	case EV_CURSOR:
		n = (*ev->func)(EV_CHECK, ev);
		if(push_key & KEY_LEFT){
			if(n != 0){ sp_cursor(sp_confirm_answer[0]); }
		}else if(push_key & KEY_RIGHT){
			if(n != 1){ sp_cursor(sp_confirm_answer[1]); }
		}
		return 0;
	case EV_CHECK:
		return sp_hit(sp_confirm_button, 2);
	case EV_ENTER:
		SNDPLAY(SND_CURSOR);
		sp_add_rc(sp_confirm_button[ev->id], 230, 0);
		sp_add_rc(sp_confirm_answer[ev->id], 138, 0);
		break;
	case EV_LEAVE:
		sp_add_rc(sp_confirm_button[ev->id], -230, 0);
		sp_add_rc(sp_confirm_answer[ev->id], -138, 0);
		break;
	case EV_LB_DOWN:
		sp_add_rc(sp_confirm_button[ev->id], 230, 0);
		sp_add_rc(sp_confirm_answer[ev->id], 138, 0);
		break;
	case EV_LB_UP:
		sp_add_rc(sp_confirm_button[ev->id], -230, 0);
		sp_add_rc(sp_confirm_answer[ev->id], -138, 0);
		if(is_event_area(ev)){
			if(ev->id == 0){
				SNDPLAY(SND_DECIDE);
			}else{
				SNDPLAY(SND_CANCEL);
			}
			yes_no = (ev->id == 0) ? YES : NO;
		}
		break;
	}
	return -1;
}

OBJ( confirm_task )
{
	static uchar* message[10] = {
		"ƒQ[ƒ€‚ğI—¹‚µ‚Ü‚·",
		"ƒ^ƒCƒgƒ‹‚É–ß‚è‚Ü‚·",
		"ƒƒCƒ“ƒƒjƒ…[‚É–ß‚è‚Ü‚·",
		"ƒ[ƒh‚µ‚Ü‚·",
		"ƒZ[ƒu‚µ‚Ü‚·",
		"ƒf[ƒ^‚ğã‘‚«‚µ‚Ü‚·",
		"ƒf[ƒ^‚ğíœ‚µ‚Ü‚·",
		"İ’è‚ğƒfƒtƒHƒ‹ƒg‚É–ß‚µ‚Ü‚·",
		"ƒNƒŠƒAƒf[ƒ^‚ğ•Û‘¶‚µ‚Ü‚·",
		"BGM‚Ìƒ~ƒ…[ƒg‚ğ‰ğœ‚µ‚Ü‚·"
	};
	static font_color[3] = { RGB(255,255,255), RGB(255,251,214), RGB(252,255,159) };
	static edge_color[3] = { RGB(13,71,117), RGB(13,104,117), RGB(127,71,122) };
	static EVLIST event;

	uint tick;
	int i, n;

	switch( task->phase ){
	//---------------------------------------------------
	// ƒ^ƒXƒN‰Šú‰»
	//---------------------------------------------------
	case 0:
		task_all_pause(TP_NORMAL_MASK);
		task_resume(task);

		galloc(&bt_confirm_bg, 1, 1, 24);
		gload(&bt_confirm_frame, DIR_SYS "confirm01.bmp");
		galloc(&bt_confirm_text, 800, 24*2, 32);

		n = (strlen(message[task->data]) * 23) / 2 + 1;
		init_font(23, RGB_WHITE, RGB(13,71,117), RGB(13,71,117));
		mess.font_type = 1;
		mess.font_style |= FONT_BOLD;
		mess.drop_x = mess.drop_y = 1;
		draw_text(bt_confirm_text, (800-n)/2, 0, message[task->data]);

		for(i = 0; i < 3; i++){
			init_font(23, font_color[i], edge_color[i], edge_color[i]);
			mess.font_type = 1;
			mess.font_style |= FONT_BOLD;
			mess.drop_x = mess.drop_y = 1;
			draw_text(bt_confirm_text, i*138, 24, " ‚Í‚¢ ‚¢‚¢‚¦");
		}

		sp_blt(BLEND(0));
		sp_confirm_bg = sp_def(31, SP_ON, 0, 0, bt_confirm_bg, RC(0,0,1,1));
		sp_chg_zoom(sp_confirm_bg, FL(SCREEN_W), FL(SCREEN_H));
		sp_blt(ALPHA(0));
		sp_confirm_frame = sp_defc(31, SP_ON, 400, 211, bt_confirm_frame, RC(0,0,800,58), 400, 29, NULL);
		sp_confirm_message = sp_defc(31, SP_ON, 0, 0, bt_confirm_text, RC(0,0,800,24), 400, 12, sp_confirm_frame);
		sp_confirm_button[0] = sp_defc(31, SP_ON, 285, 284, bt_confirm_frame, RC(0,58,230,39), 115, 19, NULL);
		sp_confirm_button[1] = sp_defc(31, SP_ON, 515, 284, bt_confirm_frame, RC(0,97,230,39), 115, 19, NULL);
		sp_confirm_answer[0] = sp_defc(31, SP_ON, 0, 0, bt_confirm_text, RC(0,24,69,24), 35, 12, sp_confirm_button[0]);
		sp_confirm_answer[1] = sp_defc(31, SP_ON, 0, 0, bt_confirm_text, RC(69,24,69,24), 35, 12, sp_confirm_button[1]);

		task->data = 0;
		task->phase++;
		break;
	//---------------------------------------------------
	// ƒEƒBƒ“ƒhƒE•\¦
	//---------------------------------------------------
	case 1:
		if(task->data == 0){
			move_mouse_cursor(285, 284, 200);
//			play_frame_sound("se_window_kakunin1", 0);
			reset_tick(task);
			task->data++;
		}
		tick = min(get_tick(task), 400);
		n = fast_speed(0,92,limit(tick,0,200)/200f);
		sp_chg_level(sp_confirm_bg, n);
		n = fast_speed(0,255,limit(tick-100,0,200)/200f);
		sp_chg_level(sp_confirm_frame, n);
		sp_chg_level(sp_confirm_button[0], n);
		sp_chg_level(sp_confirm_button[1], n);
		n = fast_speed(0,255,limit(tick-200,0,200)/200f);
		sp_chg_level(sp_confirm_message, n);
		sp_chg_level(sp_confirm_answer[0], n);
		sp_chg_level(sp_confirm_answer[1], n);
		if(tick < 400){ break; }
		task->data = 0;
		task->phase++;
		break;
	//---------------------------------------------------
	// ‚x‚d‚rA‚m‚n‚Ì‘I‘ğ
	//---------------------------------------------------
	case 2:
		if(task->data == 0){
			init_event(&event);
			set_event(&event, ev_confirm_button, 0);
			yes_no = CANCEL;
			task->data++;
		}
		if(!event.hold && (push_key & KEY_CANCEL)){
			yes_no = NO;
		}else{
			call_event(&event);
		}
		if(yes_no != CANCEL){
			task->data = 0;
			task->phase++;
		}
		break;
	//---------------------------------------------------
	// Œˆ’è
	//---------------------------------------------------
	case 3:
		if(task->data == 0){
			reset_tick(task);
			task->data++;
		}
		tick = min(get_tick(task), 400);
		n = fast_speed(255,0,limit(tick,0,200)/200f);
		sp_chg_level(sp_confirm_message, n);
		sp_chg_level(sp_confirm_answer[0], n);
		sp_chg_level(sp_confirm_answer[1], n);
		n = fast_speed(255,0,limit(tick-100,0,200)/200f);
		sp_chg_level(sp_confirm_frame, n);
		sp_chg_level(sp_confirm_button[0], n);
		sp_chg_level(sp_confirm_button[1], n);
		n = fast_speed(92,0,limit(tick-200,0,200)/200f);
		sp_chg_level(sp_confirm_bg, n);
		if(tick < 400){ break; }
		task->phase = -1;
		break;
	//---------------------------------------------------
	// ƒ^ƒXƒN‰ğ•ú
	//---------------------------------------------------
	case -1:
		sp_reset(sp_confirm_answer[1]);
		sp_reset(sp_confirm_answer[0]);
		sp_reset(sp_confirm_button[1]);
		sp_reset(sp_confirm_button[0]);
		sp_reset(sp_confirm_message);
		sp_reset(sp_confirm_frame);
		sp_reset(sp_confirm_bg);

		gfree(bt_confirm_text);
		gfree(bt_confirm_frame);
		gfree(bt_confirm_bg);

		task_all_resume(TP_NORMAL_MASK);
		task_end(task);
		break;
	}
}

extern void YesNo(int num)
{
	if(system.show_confirm){
		task_begin(1, confirm_task, num, 0, -1);
	}else{
		yes_no = YES;
	}
}
//===========================================================================
// ƒNƒCƒbƒNƒZ[ƒuƒtƒ@ƒCƒ‹‚Ì“ú•t‚ğæ“¾
//===========================================================================
extern void get_qsave_date()
{
	uchar buf[256];
	int i, j, k;
	int m, n;
	int cmp;

	// ƒNƒCƒbƒNƒZ[ƒuƒtƒ@ƒCƒ‹‚Ì“ú•t‚ğæ“¾
	memset(qsave_date, 0, sizeof(qsave_date));
	for(i = 0; i < QSAVE_MAX; i++){
		sprintf(buf, "%s\\save\\save%03d.dat", save_path, QSAVE_INDEX + i + 1);
		file_gettime(buf, NULL, NULL, &qsave_date[i]);
	}
	qsave_count = 0;
	for(i = 0; i < QSAVE_MAX; i++){
		qsave_order[i] = i;
		if(qsave_date[i].year > 0){
			++qsave_count;
		}
	}
	// ƒNƒCƒbƒNƒZ[ƒuƒtƒ@ƒCƒ‹‚ğ“ú•t‡‚É•À‚×‚é
	for(i = 0; i < QSAVE_MAX-1; i++){
		for(j = i + 1; j < QSAVE_MAX; j++){
			n = qsave_order[i];
			m = qsave_order[j];
			for(k = 0; k < 7; k++){
				switch( k ){
				case 0: cmp = qsave_date[n].year - qsave_date[m].year; break;
				case 1: cmp = qsave_date[n].month - qsave_date[m].month; break;
				case 2: cmp = qsave_date[n].day - qsave_date[m].day; break;
				case 3: cmp = qsave_date[n].hour - qsave_date[m].hour; break;
				case 4: cmp = qsave_date[n].min - qsave_date[m].min; break;
				case 5: cmp = qsave_date[n].sec - qsave_date[m].sec; break;
				case 6: cmp = n - m; break;
				}
				if(cmp != 0){
					if(cmp < 0){
						qsave_order[i] = m;
						qsave_order[j] = n;
					}
					break;
				}
			}
		}
	}
}
//===========================================================================
// ƒI[ƒgƒZ[ƒuƒtƒ@ƒCƒ‹‚Ì“ú•t‚ğæ“¾
//===========================================================================
extern void get_asave_date()
{
	uchar buf[256];
	int i, j, k;
	int m, n;
	int cmp;

	// ƒI[ƒgƒZ[ƒuƒtƒ@ƒCƒ‹‚Ì“ú•t‚ğæ“¾
	memset(asave_date, 0, sizeof(asave_date));
	for(i = 0; i < ASAVE_MAX; i++){
		sprintf(buf, "%s\\save\\save%03d.dat", save_path, ASAVE_INDEX + i + 1);
		file_gettime(buf, NULL, NULL, &asave_date[i]);
	}
	asave_count = 0;
	for(i = 0; i < ASAVE_MAX; i++){
		asave_order[i] = i;
		if(asave_date[i].year > 0){
			++asave_count;
		}
	}
	// ƒI[ƒgƒZ[ƒuƒtƒ@ƒCƒ‹‚ğ“ú•t‡‚É•À‚×‚é
	for(i = 0; i < ASAVE_MAX-1; i++){
		for(j = i + 1; j < ASAVE_MAX; j++){
			n = asave_order[i];
			m = asave_order[j];
			for(k = 0; k < 7; k++){
				switch( k ){
				case 0: cmp = asave_date[n].year - asave_date[m].year; break;
				case 1: cmp = asave_date[n].month - asave_date[m].month; break;
				case 2: cmp = asave_date[n].day - asave_date[m].day; break;
				case 3: cmp = asave_date[n].hour - asave_date[m].hour; break;
				case 4: cmp = asave_date[n].min - asave_date[m].min; break;
				case 5: cmp = asave_date[n].sec - asave_date[m].sec; break;
				case 6: cmp = n - m; break;
				}
				if(cmp != 0){
					if(cmp < 0){
						asave_order[i] = m;
						asave_order[j] = n;
					}
					break;
				}
			}
		}
	}
}
//===========================================================================
// ƒVƒXƒeƒ€ƒf[ƒ^‚ÌƒZ[ƒu
//===========================================================================
extern int save_system_data()
{
	uchar file[256];
	HFILE fp;

	sprintf(file, "%s\\save\\%s", save_path, SYSTEM_FILE);
	if(fp = fopen(file, "wb")){
		fwrite(&system, sizeof(system), 1, fp);
		fclose(fp);
	}
}
//===========================================================================
// ƒQ[ƒ€‰æ–Ê‚ÌƒLƒƒƒvƒ`ƒƒ
//===========================================================================
extern void screen_capture()
{
	if(screen_capture_func != NULL){
		(*screen_capture_func)();
		return;
	}
	apage(BACKSCREEN);
	sp_redraw(0x0000ffff);
	sp_draw(0x0000ffff, NULL);
	apage(SCREEN);
	sp_redraw(disp_flag);
}
//===========================================================================
// ƒQ[ƒ€ƒf[ƒ^‚ÌƒZ[ƒu
//===========================================================================
extern int save_game_data(int num)
{
	uchar file[256];
	HFILE fp;
	CGINFO cginfo;
	GINFO gi;
	uint crc;
	int work, cg;
	int x0, y0, x1, y1;
	int x, y, w, h;
	int i, n;

	// ƒTƒ€ƒlƒCƒ‹ì¬
	w = 96;
	h = 72;
	galloc(&cg, w, h, 16);
	ginfo(cg, &gi);
	// ƒTƒCƒY•âŠÔ
	x0 = SCREEN_W;
	y0 = SCREEN_H;
	while((x0 != w) || (y0 != h)){
		x1 = x0;
		y1 = y0;
		x0 = (x1 < w) ? min(x1*2, w) : max(x1/2, w);
		y0 = (y1 < h) ? min(y1*2, h) : max(y1/2, h);
		galloc(&work, x0, y0, 24);
		smooth(work, RCD(0,0,x0,y0), BACKSCREEN, RCS(0,0,x1,y1), -1);
		gcopy(BACKSCREEN, 0, 0, work, RC(0,0,x0,y0), NULL);
		gfree(work);
	}
	gcopy(cg, 0, 0, BACKSCREEN, RC(0,0,w,h), NULL);

	// ƒvƒŒƒCŠÔ
	save_data.play_time += getcount(10) / 1000;
	setcount(10, 0);

	// ƒ`ƒFƒbƒNƒTƒ€‚Ìæ“¾
	crc = crc32(&save_data, sizeof(SAVEDATA));

	// ƒtƒ@ƒCƒ‹‚Ì‘‚«‚İ
	sprintf(file, "%s\\save\\save%03d.dat", save_path, num+1);
	if(!(fp = fopen(file, "wb"))){
		gfree(cg);
		return -1;
	}
	fwrite(&save_data, sizeof(SAVEDATA), 1, fp);
	fwrite(&crc, sizeof(crc), 1, fp);
	fwrite(gi.pixel+(gi.height-1)*gi.pitch, w*h*2, 1, fp);
	fclose(fp);

	gfree(cg);

	// ƒVƒXƒeƒ€ƒf[ƒ^‚ÌƒZ[ƒu
	save_system_data();

	return 0;
}
//===========================================================================
// ƒQ[ƒ€ƒf[ƒ^‚Ìƒ[ƒh
//===========================================================================
extern int load_game_data(int num)
{
	uchar file[256];
	HFILE fp;
	uint crc;
	SAVEDATA sd;

	// ƒtƒ@ƒCƒ‹‚Ì“Ç‚İ‚İ
	sprintf(file, "%s\\save\\save%03d.dat", save_path, num+1);
	if(!(fp = fopen(file, "rb"))){
		return -1;
	}
	fread(&sd, sizeof(SAVEDATA), 1, fp);
	fread(&crc, sizeof(crc), 1, fp);
	fclose(fp);

	if(crc != crc32(&sd, sizeof(SAVEDATA))){
		return -2;
	}
	memcpy(&save_data, &sd, sizeof(SAVEDATA));
	memcpy(&save_temp, &save_data, sizeof(SAVEDATA));

	setcount(10, 0);

	return 0;
}
//===========================================================================
// ƒƒbƒZ[ƒW•\¦
//===========================================================================
int bt_notice_bg;
int bt_notice_frame;
int bt_notice_text;

SPRITE* sp_notice_bg;
SPRITE* sp_notice_frame;
SPRITE* sp_notice_text;

uchar* notice_text;
uint notice_time;

OBJ( notice_task )
{
	RECT rc;
	uint tick;
	int x, y;
	int n;

	switch( task->phase ){
	//---------------------------------------------------
	// ƒ^ƒXƒN‰Šú‰»
	//---------------------------------------------------
	case 0:
		task_all_pause(TP_NORMAL_MASK);
		task_resume(task);

		galloc(&bt_notice_bg, 1, 1, 24);
		gload(&bt_notice_frame, DIR_SYS "confirm01.bmp");
		galloc(&bt_notice_text, 800, 24, 32);

		n = (strlen(notice_text) * 23) / 2 + 1;
		init_font(23, RGB_WHITE, RGB(13,71,117), RGB(13,71,117));
		mess.font_type = 1;
		mess.font_style |= FONT_BOLD;
		mess.drop_x = mess.drop_y = 1;
		draw_text(bt_notice_text, (800-n)/2, 0, notice_text);

		sp_blt(BLEND(0));
		sp_notice_bg = sp_def(31, SP_ON, 0, 0, bt_notice_bg, RC(0,0,1,1));
		sp_chg_zoom(sp_notice_bg, FL(SCREEN_W), FL(SCREEN_H));
		sp_blt(ALPHA(0));
		sp_notice_frame = sp_defc(31, SP_ON, 400, 211, bt_notice_frame, RC(0,0,800,58), 400, 29, NULL);
		sp_notice_text = sp_defc(31, SP_ON, 0, 0, bt_notice_text, RC(0,0,800,24), 400, 12, sp_notice_frame);

		task->data = 0;
		task->phase++;
		break;
	//---------------------------------------------------
	// ƒƒbƒZ[ƒW•\¦
	//---------------------------------------------------
	case 1:
		if(task->data == 0){
			reset_tick(task);
			task->data = 1;
		}
		tick = min(get_tick(task), 400);
		n = fast_speed(0,92,limit(tick,0,200)/200f);
		sp_chg_level(sp_notice_bg, n);
		n = fast_speed(0,255,limit(tick-100,0,200)/200f);
		sp_chg_level(sp_notice_frame, n);
		n = fast_speed(0,255,limit(tick-200,0,200)/200f);
		sp_chg_level(sp_notice_text, n);
		if(tick < 400){ break; }
		task->data = 0;
		task->phase++;
		break;
	//---------------------------------------------------
	// ƒNƒŠƒbƒN‘Ò‚¿
	//---------------------------------------------------
	case 2:
		if(task->data == 0){
			reset_tick(task);
			task->data = 1;
		}
		if((get_tick(task) >= notice_time) || (push_key & (KEY_DECIDE|KEY_CANCEL))){
			task->data = 0;
			task->phase++;
		}
		break;
	//---------------------------------------------------
	// ƒƒbƒZ[ƒWÁ‹
	//---------------------------------------------------
	case 3:
		if(task->data == 0){
			reset_tick(task);
			task->data = 1;
		}
		tick = min(get_tick(task), 400);
		n = fast_speed(255,0,limit(tick,0,200)/200f);
		sp_chg_level(sp_notice_text, n);
		n = fast_speed(255,0,limit(tick-100,0,200)/200f);
		sp_chg_level(sp_notice_frame, n);
		n = fast_speed(92,0,limit(tick-200,0,200)/200f);
		sp_chg_level(sp_notice_bg, n);
		if(tick < 400){ break; }
		task->phase = -1;
		break;
	//---------------------------------------------------
	// ƒ^ƒXƒN‰ğ•ú
	//---------------------------------------------------
	case -1:
		sp_reset(sp_notice_text);
		sp_reset(sp_notice_frame);
		sp_reset(sp_notice_bg);
		gfree(bt_notice_text);
		gfree(bt_notice_frame);
		gfree(bt_notice_bg);
		task_all_resume(TP_NORMAL_MASK);
		task_end(task);
		break;
	}
}

extern void notice(uchar* text, uint time)
{
	if(system.show_notice){
		notice_text = text;
		notice_time = (time == 0) ? 0x7fffffff : time;
		task_begin(2, notice_task, NULL, 0, -1);
	}
}
//===========================================================================
// •¶š—ñ’uŠ·
//===========================================================================
extern void replace_str(uchar* dest, uchar* src, uchar* key, uchar* repl)
{
	int src_len = strlen(src);
	int key_len = strlen(key);
	int repl_len = strlen(repl);

	while(src_len >= key_len){
		if(memcmp(src, key, key_len) == 0){
			strcpy(dest, repl);
			dest += repl_len;
			src += key_len;
			src_len -= key_len;
		}else{
			if(ISKANJI(*src)){
				*dest++ = *src++;
				src_len--;
			}
			*dest++ = *src++;
			src_len--;
		}
	}
	strcpy(dest, src);
	*dest = '\0';
}
//===========================================================================
// •¶š—ñŒŸõ
//===========================================================================
extern int search_str(uchar* src, uchar* key)
{
	int src_len = strlen(src);
	int key_len = strlen(key);
	int len = src_len - key_len;
	int i;
	uchar* p;

	for(i = 0; i <= len; i++){
		p = &src[i];
		if(memcmp(p, key, key_len) == 0){
			return i;
		}
		if(ISKANJI(*p)){ ++i; }
	}
	return -1;
}
//===========================================================================
// ”¼ŠpƒeƒLƒXƒg¨‘SŠpƒeƒLƒXƒg•ÏŠ·
//===========================================================================
uchar* zen = "@IHcBuvA‚ğ‚Ÿ‚¡‚£‚¥‚§‚á‚ã‚å‚Á[‚ ‚¢‚¤‚¦‚¨‚©‚«‚­‚¯‚±"
			"‚³‚µ‚·‚¹‚»‚½‚¿‚Â‚Ä‚Æ‚È‚É‚Ê‚Ë‚Ì‚Í‚Ğ‚Ó‚Ö‚Ù‚Ü‚İ‚Ş‚ß‚à‚â‚ä‚æ‚ç‚è‚é‚ê‚ë‚í‚ñ";
uchar* han = "\xa0!?¥¡¢£¤¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜ?";

extern void convert_htoz(uchar* dest, uchar* src)
{
	while (*src)
	{
		if (ISKANJI(*src))
		{
			// Ë«×Ö½Ú×Ö·û£¨GBK±àÂë£©
			*dest++ = *src++;
			*dest++ = *src++;
		}
		else if (*src == 0x1b)
		{
			// ×ªÒå×Ö·û
			++src;
			*dest++ = *src++;
		}
		else
		{
			// µ¥×Ö½Ú×Ö·û
			*dest++ = *src++;
		}
	}
	*dest = '\0';
}

//===========================================================================
// ƒXƒNƒŠƒvƒg”Ô†‚Ìæ“¾
//===========================================================================
extern int get_script_index(uchar* file)
{
	int i;

	if((file != NULL) && (file[0] != '\0')){
		for(i = 1; i < script_dat_cnt; i++){
			if(strcmpi(script_dat[i].file, file) == 0){
				return i;
			}
		}
	}
	return 0;
}
//===========================================================================
// •Ï””Ô†‚Ìæ“¾
//===========================================================================
extern int get_var_index(uchar* name)
{
	int i;

	if((name != NULL) && (name[0] != '\0')){
		for(i = 1; i < var_dat_cnt; i++){
			if(strcmpi(var_dat[i].name, name) == 0){
				return i;
			}
		}
	}
	return 0;
}
//===========================================================================
// •Ï”‚Ìæ“¾
//===========================================================================
extern int get_variable(uchar* name)
{
	int i = get_var_index(name);

	if(i != 0){
		i = var_dat[i].index;
		if(i >= 0 && i <= 1024){
			if(i < 128){
				return system.var[i];
			}else{
				return save_temp.var[i];
			}
		}
	}
	return 0;
}
//===========================================================================
// •Ï”‚Ìİ’è
//===========================================================================
extern void set_variable(uchar* name, int value)
{
	int i = get_var_index(name);

	if(i != 0){
		i = var_dat[i].index;
		if(i >= 0 && i <= 1024){
			if(i < 128){
				system.var[i] = value;
			}else{
				save_temp.var[i] = value;
			}
		}
	}
}
//===========================================================================
// ƒCƒxƒ“ƒg”Ô†‚Ìæ“¾
//===========================================================================
extern int get_ev_index(uchar* name)
{
	int i;

	if((name != NULL) && (name[0] != '\0')){
		for(i = 1; i < ev_dat_cnt; i++){
			if(strcmpi(ev_dat[i].name, name) == 0){
				return i;
			}
		}
	}
	return 0;
}
//===========================================================================
// ”wŒi”Ô†‚Ìæ“¾
//===========================================================================
extern int get_bg_index(uchar* name)
{
	int i;

	if((name != NULL) && (name[0] != '\0')){
		for(i = 1; i < bg_dat_cnt; i++){
			if(strcmpi(bg_dat[i].name, name) == 0){
				return i;
			}
		}
	}
	return 0;
}
//===========================================================================
// —§‚¿”Ô†‚Ìæ“¾
//===========================================================================
extern int get_st_index(uchar* name)
{
	int i;

	if((name != NULL) && (name[0] != '\0')){
		for(i = 1; i < st_dat_cnt; i++){
			if(strcmpi(st_dat[i].name, name) == 0){
				return i;
			}
		}
	}
	return 0;
}
//===========================================================================
// BGM”Ô†‚Ìæ“¾
//===========================================================================
extern int get_bgm_index(uchar* name)
{
	int i;

	if((name != NULL) && (name[0] != '\0')){
		for(i = 1; i < bgm_dat_cnt; i++){
			if(strcmpi(bgm_dat[i].name, name) == 0){
				return i;
			}
		}
	}
	return 0;
}
//===========================================================================
// ŠÂ‹«‰¹”Ô†‚Ìæ“¾
//===========================================================================
extern int get_amb_index(uchar* name)
{
	int i;

	if((name != NULL) && (name[0] != '\0')){
		for(i = 1; i < amb_dat_cnt; i++){
			if(strcmpi(amb_dat[i].name, name) == 0){
				return i;
			}
		}
	}
	return 0;
}
//===========================================================================
// Œø‰Ê‰¹”Ô†‚Ìæ“¾
//===========================================================================
extern int get_se_index(uchar* name)
{
	int i;

	if((name != NULL) && (name[0] != '\0')){
		for(i = 1; i < se_dat_cnt; i++){
			if(strcmpi(se_dat[i].name, name) == 0){
				return i;
			}
		}
	}
	return 0;
}
//===========================================================================
// Œø‰Ê‰¹ƒtƒ@ƒCƒ‹–¼‚Ìæ“¾
//===========================================================================
extern uchar* get_se_file(uchar* name)
{
	int i;

	if((name != NULL) && (name[0] != '\0')){
		for(i = 1; i < se_dat_cnt; i++){
			if(strcmpi(se_dat[i].name, name) == 0){
				return se_dat[i].file;
			}
		}
	}
	return "";
}
//===========================================================================
// ‰æ‘œƒf[ƒ^‚Ìæ“¾
//===========================================================================
extern void get_cg_info(int id, CGINFO* cginfo)
{
	if(cginfo == NULL){ return; }

	memset(cginfo, 0, sizeof(CGINFO));
	cginfo->kind = id / CG_MASK;
	if((cginfo->kind < 0) || (cginfo->kind >= CG_MAX)){
		cginfo->kind = CG_ERR;
		return;
	}
	cginfo->index = id % CG_MASK;
	switch( cginfo->kind ){
	case CG_BG:
		if((cginfo->index > 0) && (cginfo->index < bg_dat_cnt)){
			strcpy(cginfo->name, bg_dat[cginfo->index].name);
			strcpy(cginfo->file, bg_dat[cginfo->index].file);
			strcpy(cginfo->option, bg_dat[cginfo->index].option);
			cginfo->color = bg_dat[cginfo->index].color;
			cginfo->order = bg_dat[cginfo->index].order;
			cginfo->link = bg_dat[cginfo->index].link;
			cginfo->loop = true;
			return;
		}
		break;
	case CG_EV:
		if((cginfo->index > 0) && (cginfo->index < ev_dat_cnt)){
			strcpy(cginfo->name, ev_dat[cginfo->index].name);
			strcpy(cginfo->file, ev_dat[cginfo->index].file);
			strcpy(cginfo->option, ev_dat[cginfo->index].option);
			cginfo->color = ev_dat[cginfo->index].color;
			cginfo->order = ev_dat[cginfo->index].order;
			cginfo->link = ev_dat[cginfo->index].link;
			cginfo->loop = true;
			return;
		}
		break;
	case CG_ST:
		if((cginfo->index > 0) && (cginfo->index < st_dat_cnt)){
			strcpy(cginfo->name, st_dat[cginfo->index].name);
			strcpy(cginfo->file, st_dat[cginfo->index].file);
			strcpy(cginfo->option, st_dat[cginfo->index].option);
			cginfo->order = st_dat[cginfo->index].order;
			cginfo->link = st_dat[cginfo->index].link;
			cginfo->loop = true;
			return;
		}
		break;
	case CG_EFX:
		if((cginfo->index > 0) && (cginfo->index < efx_dat_cnt)){
			strcpy(cginfo->name, efx_dat[cginfo->index].name);
			strcpy(cginfo->file, efx_dat[cginfo->index].file);
			strcpy(cginfo->option, "mot");
			cginfo->x = efx_dat[cginfo->index].x;
			cginfo->y = efx_dat[cginfo->index].y;
			cginfo->loop = efx_dat[cginfo->index].loop;
			return;
		}
		break;
	}
	cginfo->kind = CG_ERR;
	cginfo->index = 0;
}
//===========================================================================
// ƒVƒXƒeƒ€ƒf[ƒ^‚Ì‰Šú‰»
//===========================================================================
extern void init_system_data()
{
	int i;

	system.read_skip = 1;
	system.skip_menu = 0;
	system.auto_menu = 0;
	system.skip_speed = 0;
	system.text_speed = 75;
	system.auto_speed = 75;
	system.frame_opaque = 75;
	system.show_confirm = true;
	system.show_notice = true;
	system.show_flowchart = true;
	system.voice_cut = true;
	system.mouse_assist = false;
	system.mouse_hide = false;
	system.right_assign = 0;
	system.ctrl_lock = false;
	system.global_focus = false;
	system.global_sound = false;
	system.effect = true;
	system.master_flag = true;
	system.music_flag = true;
	system.ambient_flag = true;
	system.sound_flag = true;
	system.voice_flag = true;
	system.chars_flag = 0xffffffff;
	system.master_vol = 75;
	system.music_vol = 100;
	system.ambient_vol = 100;
	system.sound_vol = 100;
	system.voice_vol = 100;
	for(i = 0; i < 32; i++){
		system.chars_vol[i] = 100;
	}
}
//===========================================================================
// ƒZ[ƒuƒf[ƒ^‚Ì‰Šú‰»
//===========================================================================
extern void init_save_data()
{
	int i;

	memset(&save_data, 0, sizeof(save_data));
	save_data.script[0] = 1;
	for(i = 0; i < ADV_LAYER_MAX; i++){
		init_layer_param(i);
	}
	save_data.cg.layer[0].info.id = get_bg_index("black");
	save_data.frame = 1;
	save_data.music_vol = 100;
	save_data.ambient_vol = 100;
	save_data.sound_vol = 100;
	save_data.voice_vol = 100;
	save_data.mode = MODE_ADV;
}
//===========================================================================
// ƒZ[ƒuƒf[ƒ^‚Ì‘Ò”ğ
//===========================================================================
extern void push_save_data()
{
	memcpy(&save_data, &save_temp, sizeof(save_temp));
}
//===========================================================================
// ƒZ[ƒuƒf[ƒ^‚Ì•œ‹A
//===========================================================================
extern void pop_save_data()
{
	memcpy(&save_temp, &save_data, sizeof(save_data));
}
//===========================================================================
// ƒZ[ƒuƒf[ƒ^‚Ìˆø‚«Œp‚¬
//===========================================================================
extern void preserve_save_data()
{
	init_save_data();

	// ƒNƒŠƒA‰ñ”
	save_data.clear_count = save_temp.clear_count;
	// ƒvƒŒƒCŠÔ
	save_data.play_time = save_temp.play_time;
}
//===========================================================================
// ƒŒƒCƒ„[î•ñ‚Ì‰Šú‰»
//===========================================================================
extern void init_layer_param(uint layer_id)
{
	if(layer_id < ADV_LAYER_MAX){
		save_temp.cg.layer[layer_id].info.id = 0;
		save_temp.cg.layer[layer_id].info.x = 0;
		save_temp.cg.layer[layer_id].info.y = 0;
		save_temp.cg.layer[layer_id].info.opaque = 255;
		save_temp.cg.layer[layer_id].info.zx = 65536;
		save_temp.cg.layer[layer_id].info.zy = 65536;
		save_temp.cg.layer[layer_id].info.rot = 0;
	}
}
//===========================================================================
// ƒŒƒCƒ„[î•ñ‚Ìİ’è
//===========================================================================
extern void set_layer_param(uint layer_id, int param_id, int value)
{
	if((layer_id >= 0) && (layer_id < ADV_LAYER_MAX)){
		switch( param_id ){
		case CV_ID: save_temp.cg.layer[layer_id].info.id = value; break;
		case CV_X: save_temp.cg.layer[layer_id].info.x = value; break;
		case CV_Y: save_temp.cg.layer[layer_id].info.y = value; break;
		case CV_ALPHA: save_temp.cg.layer[layer_id].info.opaque = limit(value,0,255); break;
		case CV_ZX: save_temp.cg.layer[layer_id].info.zx = max(value,0); break;
		case CV_ZY: save_temp.cg.layer[layer_id].info.zy = max(value,0); break;
		case CV_ROT: save_temp.cg.layer[layer_id].info.rot = value; break;
		}
	}
}
//===========================================================================
// ƒŒƒCƒ„[î•ñ‚Ìæ“¾
//===========================================================================
extern int get_layer_param(uint layer_id, int param_id)
{
	if(layer_id < ADV_LAYER_MAX){
		switch( param_id ){
		case CV_ID: return save_temp.cg.layer[layer_id].info.id;
		case CV_X: return save_temp.cg.layer[layer_id].info.x;
		case CV_Y: return save_temp.cg.layer[layer_id].info.y;
		case CV_ALPHA: return save_temp.cg.layer[layer_id].info.opaque;
		case CV_ZX: return save_temp.cg.layer[layer_id].info.zx;
		case CV_ZY: return save_temp.cg.layer[layer_id].info.zy;
		case CV_ROT: return save_temp.cg.layer[layer_id].info.rot;
		}
	}
	return 0;
}
//===========================================================================
// ƒXƒNƒŠ[ƒ“Ø‚è‘Ö‚¦ŠÄ‹
//===========================================================================
OBJ( screen_mode_task )
{
	switch( task->phase ){
	case 0: task->state |= T_SAVE; task->phase++; break;
	case 1: system.full_screen = getsystem("fullscreen"); break;
	case -1: task_end(task); break;
	}
}
//===========================================================================
// ƒ}ƒEƒXƒJ[ƒ\ƒ‹‹­§ˆÚ“®
//===========================================================================
typedef struct{
	int sx, sy;
	int ex, ey;
	uint time;
} MMCWORK;

OBJ( move_mouse_cursor_task )
{
	MMCWORK* w = task->work;
	int x, y;
	uint tick;

	switch( task->phase ){
	case 0:
		task->data = 0;
		task->phase++;
		break;
	case 1:
		if(task->data == 0){
			reset_tick(task);
			task->data = 1;
		}
		if((mouse_x >= 0 && mouse_x < SCREEN_W) &&
			(mouse_y >= 0 && mouse_y < SCREEN_H)){
			tick = min(get_tick(task), w->time);
			if(w->time > 0){
				x = slow_speed(w->sx, w->ex, (float)tick/w->time);
				y = slow_speed(w->sy, w->ey, (float)tick/w->time);
				cur_setpos(x, y);
			}else{
				cur_setpos(w->ex, w->ey);
			}
			if(tick < w->time){ break; }
		}
		task->phase = -1;
		break;
	case -1:
		task_end(task);
		break;
	}
}
extern void move_mouse_cursor(int x, int y, uint time)
{
	if(system.mouse_assist){
		TASK* task = task_begin(0, move_mouse_cursor_task, NULL, 0, 0);
		if(task != NULL){
			MMCWORK* w = task->work;
			w->sx = mouse_x;
			w->sy = mouse_y;
			w->ex = x;
			w->ey = y;
			w->time = time;
			task_init_call(task);
		}
	}
}
//===========================================================================
// ƒp[ƒc”Ô†”z—ñ‚Ìæ“¾
//===========================================================================
extern uchar* get_parts_state(int id, uchar* parts_state_max)
{
	static uchar parts_state[256];
	int i;

	memset(parts_state, 0, sizeof(parts_state));
	for(i = 0; i < parts_list_count; ++i){
		parts_state[parts_index_list[i]] = parts_state_list[i];
	}
	parts_list_count = 0;
	return parts_state;
}
//===========================================================================
// ƒp[ƒcƒIƒvƒVƒ‡ƒ“‚Ìİ’è
//===========================================================================
extern bool set_parts_option(uchar* option)
{
	static uchar* key[2] = { "p", "s" };
	int key_no = -1;
	int parts_no = -1;
	int i, len;

	parts_list_count = 0;

	uchar* p = get_token(option);
	if(strcmpi(token, "mot") == 0){
		return false;
	}

	while(token_type != _NULL_){
		if(token_type == _KEYWORD_){
			len = strlen(token);
			if(len > 1){
				i = 1;
				while(char_type[token[i]] & (C_SET|C_KANJI)){
					if(char_type[token[i]] & C_KANJI){ ++i; }
					++i;
				}
				p -= (len - i);
				token[i] = '\0';
			}
			key_no = -1;
			for(i = 0; i < sizeof(key)/sizeof(key[0]); ++i){
				if(strcmpi(token, key[i]) == 0){
					key_no = i;
					break;
				}
			}
		}else if(token_type == _CONST_){
			switch( key_no ){
			case 0: 
				parts_no = token_value;
				key_no = 1;
				break;
			case 1:
				if(parts_no >= 0){
					parts_index_list[parts_list_count] = parts_no;
					parts_state_list[parts_list_count] = token_value;
					++parts_list_count;
					parts_no = -1;
				}
				break;
			}
		}else{
			if((token_type != _SYMBOL_) || ((token[0] != ':') && (token[0] != '='))){
				parts_no = -1;
				key_no = -1;
			}
		}
		p = get_token(p);
	}

	return true;
}
//===========================================================================
// ƒvƒƒOƒ‰ƒ€ƒGƒ“ƒgƒŠ[
//===========================================================================
#define MENU_TOP				0
#define MENU_FILE				1
#define MENU_SCREEN				2
#define MENU_FILE_EXIT			40001
#define MENU_FILE_TITLE			40002
#define MENU_SCREEN_WINDOW		40003
#define MENU_SCREEN_FULL		40004

uchar* trial_name[5] = {
	"",
	"Trial",
	"TechGian",
	"Push",
	"MegaStore"
};

void main()
{
	bool game_loop = true;
	uchar buf[256];
	uchar* file;
	int i, j, n;

	menu_init();
	menu_add(MENU_TOP, MENU_FILE, "Ì§²Ù(&F)", 0);
	menu_add(MENU_FILE, MENU_FILE_TITLE, "À²ÄÙÍ–ß‚?&R)", 0);
	menu_add(MENU_FILE, -1, "-", 0);
	menu_add(MENU_FILE, MENU_FILE_EXIT, "I—¹(&X)", 0);
	menu_add(MENU_TOP, MENU_SCREEN, "‰æ–Ê(&S)", 0);
	menu_add(MENU_SCREEN, MENU_SCREEN_FULL, "ÌÙ½¸Ø°?&F)", 1);
	menu_add(MENU_SCREEN, MENU_SCREEN_WINDOW, "³¨İÄŞ³‰æ–Ê(&W)", 1);
	menu_build();
	menu_draw();

	sp_flag(SP_ONCE);

	dir_get(0x05, save_path);
	strcat(save_path, "ESCUDE");
	dir_create(save_path);
	strcat(save_path, "\\" DIR_SAVE);
	strcat(save_path, trial_name[TRIAL]);
	dir_create(save_path);

	sprintf(buf, "%s\\save", save_path);
	dir_create(buf);

	SETFONT(0, "‚l‚r ƒSƒVƒbƒN", 24, 0, FONT_ANTIALIAS);
	SETFONT(1, "‚l‚r ƒSƒVƒbƒN", 12, 0, FONT_ANTIALIAS);

	galloc(SCREEN, SCREEN_W, SCREEN_H, 24);
	galloc(BACKSCREEN, SCREEN_W, SCREEN_H, 24);

	memset(&system, 0, sizeof(system));
	init_system_data();
	system.flow_scale = 40960;

	sprintf(buf, "%s\\save\\%s", save_path, SYSTEM_FILE);
	sysfile(buf, &system, sizeof(system));

	system.name_flag |= 0x0000ffff;
	system.show_confirm = true;
	system.show_notice = true;
	system.ctrl_lock = false;
	system.effect = true;
	system.global_sound = system.global_focus;
	setsystem("global_focus", system.global_focus);
	setsystem("global_sound", system.global_sound);
	setsystem("fullscreen", system.full_screen);
	enter_block(1);

	uchar init_voice_group[100] = {
	//  y0, y1, y2, y3, y4, y5, y6, y7, y8, y9
		10,  0,  3,  1,  2,  5,  7,  6,  8, 10, // 0x
		 4, 10, 10, 10, 10, 10, 10, 10, 10, 10, // 1x
		10, 10, 10, 10, 10, 10, 10, 10, 10, 10, // 2x
		10, 10, 10, 10, 10, 10, 10, 10, 10, 10, // 3x
		10, 10, 10, 10, 10, 10, 10, 10, 10, 10, // 4x
		10, 10, 10, 10, 10, 10, 10, 10, 10, 10, // 5x
		10, 10, 10, 10, 10, 10, 10, 10, 10,  9, // 6x
		10, 10, 10, 10, 10, 10, 10, 10, 10, 10, // 7x
		10, 10, 10, 10, 10, 10, 10, 10, 10, 10, // 8x
		10, 10, 10, 10, 10, 10, 10, 10, 10, 10  // 9x
	};
	memcpy(voice_group, init_voice_group, sizeof(init_voice_group));

	get_qsave_date();
	get_asave_date();

	inner_music_vol = 100;
	inner_ambient_vol = 100;
	inner_sound_vol = 100;
	inner_voice_vol = 100;
	for(i = 0; i < 100; i++){
		fade_volume[i] = 100;
	}

	music_fx = 0;
	ambient_fx = 0;
	sound_fx = 0;
	voice_fx = 0;

	music_num = 0;
	for(i = 0; i < SND_VOC_MAX; i++){
		voice_num[i] = 0;
	}

	hide_cursor_mode(system.mouse_hide);

	token_init();
	task_init();
	task_begin(31, screen_mode_task, NULL, 0, -1);
	disp_flag = SP_ALL;

	init_launch_app();

	load_soundfx();
	init_script_literal();

	set_parts_state_func(get_parts_state);

	SNDLOAD(SND_CURSOR, "sys_cursor");
	SNDLOAD(SND_CLICK, "sys_click");
	SNDLOAD(SND_DECIDE, "sys_kettei");
	SNDLOAD(SND_CANCEL, "sys_cancel");

#define DISPATCH_BEGIN			switch( game_mode ){
#define DISPATCH(mode, script)	case mode: game_loop = call(script); break;
#define DISPATCH_END			}

//	frame_skip = true;
	game_mode = MODE_LOGO;

	while( game_loop ){
		DISPATCH_BEGIN
		DISPATCH( MODE_LOGO, "logo.c" )
		DISPATCH( MODE_OPENING, "opening.c" )
		DISPATCH( MODE_TITLE, "title.c" )
		DISPATCH( MODE_ADV, "adv.c" )
		DISPATCH( MODE_ADV_RP, "adv.c" )
		DISPATCH( MODE_EXTRA, "extra.c" )
		DISPATCH( MODE_GRAPHICS, "extra_graphic.c" )
		DISPATCH( MODE_SCENE, "extra_scene.c" )
		DISPATCH( MODE_MUSIC, "extra_music.c" )
		DISPATCH( MODE_STAFF, "extra_staff.c" )
		DISPATCH( MODE_MOVIE, "extra_movie.c" )
		DISPATCH( MODE_CREDIT0, "credit0.c" )
		DISPATCH( MODE_CREDIT1, "credit1.c" )
		DISPATCH( MODE_ENDING, "ending.c" )
		DISPATCH( MODE_ADV_DEBUG, "adv_debug.c" )
		DISPATCH_END
	}
	exit();
}
