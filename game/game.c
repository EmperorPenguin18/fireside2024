#include <stdlib.h>
#include <stdio.h>

#define object int

int turn = 0;
object tiles[5][5] = {0};
int tiles_type[5][5] = {0}; // 0 grass, 1 weed, 2 dirt
int mode = 0;
object selector = 0;
int sel_x = 0;
int sel_y = 0;
int sel_rot = 0;
object text = 0;
object bunnies[25] = {0};
int bunnies_x[25] = {0};
int bunnies_y[25] = {0};
int bunny_count = 0;
const int AP = 2;

static void bunny() {
	for (int i = 0; i < bunny_count; i++) {
		int new_x = bunnies_x[i];
		int new_y = bunnies_y[i];
		for (int n = 0; n < AP; n++) {
			int close_x = 100;
			int close_y = 100;
			for (int j = 0; j < 5; j++) {
			  for (int k = 0; k < 5; k++) {
			    if (tiles_type[j][k] == 1) {
			      if (abs(j - bunnies_x[i]) < close_x ||
				abs(k - bunnies_y[i]) < close_y) {
			        close_x = j;
				close_y = k;
			      }
			    }
			  }
			}
			if (close_x > new_x) {
				new_x = new_x + 1;
			} else if (close_x < new_x) {
				new_x = new_x - 1;
			} else if (close_y > new_y) {
				new_y = new_y + 1;
			} else if (close_y < new_y) {
				new_y = new_y - 1;
			} else {//eat
				tiles_type[new_x][new_y] = 2;
				tidal.set_sprite(tiles[new_x][new_y], "dirt.png");
			}
		}
		bunnies_x[i] = new_x;
		bunnies_y[i] = new_y;
		tidal.set_pos(bunnies[i], new_x*32, new_y*32);
	}
	if ((turn-2) % 5 == 0) {
		bunnies[bunny_count] = tidal.create();
		int rand_x = rand() % 5;
		int rand_y = rand() % 5;
		bunnies_x[bunny_count] = rand_x;
		bunnies_y[bunny_count] = rand_y;
		tidal.set_pos(bunnies[bunny_count], rand_x*32, rand_y*32);
		tidal.set_size(bunnies[bunny_count], 32, 32);
		tidal.set_sprite(bunnies[bunny_count], "bunny.png");
		bunny_count = bunny_count + 1;
	}
}

static void mode0(const int key) {
	if (key == 262) { //right
		if (sel_x < 4) {
			sel_x = sel_x + 1;
		}
		tidal.set_pos(selector, sel_x*32, sel_y*32);
	} else if (key == 263) { //left
		if (sel_x > 0) {
			sel_x = sel_x - 1;
		}
		tidal.set_pos(selector, sel_x*32, sel_y*32);
	} else if (key == 264) { //down
		if (sel_y < 4) {
			sel_y = sel_y + 1;
		}
		tidal.set_pos(selector, sel_x*32, sel_y*32);
	} else if (key == 265) { //up
		if (sel_y > 0) {
			sel_y = sel_y - 1;
		}
		tidal.set_pos(selector, sel_x*32, sel_y*32);
	} else if (key == 90) { //z
		if (tiles_type[sel_x][sel_y] == 1) {
			mode = 1;
			tidal.set_sprite(selector, "arrow.png");
		}
	} else if (key == 88) { //x
		//nothing
	} else if (key == 81) { //q
		tidal.quit();
	}
}

static void pass_turn() {
	mode = 0;
	turn = turn + 1;
	tidal.set_text(text, "Turn: %d", turn);
	bunny();
	tidal.set_sprite(selector, "selector.png");
}

static void mode1(const int key) {
	if (key == 262) { //right
		sel_rot = 0;
		tidal.set_rotation(selector, sel_rot);
	} else if (key == 263) { //left
		sel_rot = 180;
		tidal.set_rotation(selector, sel_rot);
	} else if (key == 264) { //down
		sel_rot = 90;
		tidal.set_rotation(selector, sel_rot);
	} else if (key == 265) { //up
		sel_rot = 270;
		tidal.set_rotation(selector, sel_rot);
	} else if (key == 90) { //z
		int sel_x_new = sel_x;
		int sel_y_new = sel_y;
		if (sel_rot == 0 && sel_x != 4) {
			sel_x_new = sel_x_new + 1;
		} else if (sel_rot == 90 && sel_y != 4) {
			sel_y_new = sel_y_new + 1;
		} else if (sel_rot == 180 && sel_x != 0) {
			sel_x_new = sel_x_new - 1;
		} else if (sel_rot == 270 && sel_y != 0) {
			sel_y_new = sel_y_new - 1;
		}
		if (tiles_type[sel_x_new][sel_y_new] == 0) {
			tiles_type[sel_x_new][sel_y_new] = 1;
			tidal.set_sprite(tiles[sel_x_new][sel_y_new], "weed.png");
			pass_turn();
		} else if (tiles_type[sel_x_new][sel_y_new] == 2) {
			tiles_type[sel_x_new][sel_y_new] = 0;
			tidal.set_sprite(tiles[sel_x_new][sel_y_new], "grass.png");
			pass_turn();
		}
	} else if (key == 88) { //x
		mode = 0;
		tidal.set_sprite(selector, "selector.png");
	} else if (key == 81) { //q
		tidal.quit();
	}
}

static void keydown(const int key) {
	if (mode == 0) {
		mode0(key);
	} else {
		mode1(key);
	}
}

void script() {
	tidal.set_music("music.ogg");
	tidal.set_background_colour(255, 153, 102, 255);
	tidal.set_cb_keydown(keydown);

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			tiles[i][j] = tidal.create();
			tidal.set_pos(tiles[i][j], i*32, j*32);
			tidal.set_size(tiles[i][j], 32, 32);
			tidal.set_sprite(tiles[i][j], "grass.png");
			tiles_type[i][j] = 0;
		}
	}
	tidal.set_sprite(tiles[2][2], "weed.png");
	tiles_type[2][2] = 1;

	selector = tidal.create();
	tidal.set_pos(selector, sel_x*32, sel_y*32);
	tidal.set_size(selector, 32, 32);
	tidal.set_sprite(selector, "selector.png");

	text = tidal.create();
	tidal.set_pos(text, 170, 12);
	tidal.set_font(text, "fibberish.ttf");
	tidal.set_text(text, "Turn: %d", turn);
}
