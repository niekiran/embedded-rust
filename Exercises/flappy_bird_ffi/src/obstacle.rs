use crate::color;
use crate::config::*;
use crate::display;

pub struct Obstacle {
    x_top: Coord,
    y_top: Coord,
    x_btm: Coord,
    y_btm: Coord,
    speed: u32,
    height_top: u32,
    height_btm: u32,
     pub already_scored: bool,
}

impl Obstacle {
    pub fn init() -> Self {
        Obstacle {
            x_top: 240,
            y_top: 0 + SCORE_BOARD_HEIGHT as Coord,
            x_btm: 240,
            y_btm: 180,
            speed: SPEED,
            height_top: 100,
            height_btm: 60 - PLANTS_HEIGHT,
            already_scored:  false,
        }
    }

    fn draw(&self) {
        self.draw_top();
        self.draw_bottom();
    }

    fn clear_top(&self, x: Coord, width: u32) {
        display::draw_rect_angle(x, width, self.y_top, self.height_top, color::BACKGROUND);
    }

    fn clear_bottom(&self, x: Coord, width: u32) {
        display::draw_rect_angle(x, width, self.y_btm, self.height_btm, color::BACKGROUND);
    }

    fn clear(&self) {
        self.clear_top(self.x_top + OBSTACLE_WIDTH as Coord, self.speed);
        self.clear_bottom(self.x_btm + OBSTACLE_WIDTH as Coord, self.speed);

        if self.x_top <= LCD_BIGIN {
            self.clear_top(LCD_BIGIN, OBSTACLE_WIDTH);
        }

        if self.x_btm <= LCD_BIGIN {
            self.clear_bottom(LCD_BIGIN, OBSTACLE_WIDTH);
        }
    }

    fn draw_top(&self) {
        display::draw_rect_angle(
            self.x_top,
            OBSTACLE_WIDTH,
            self.y_top,
            self.height_top,
            color::BLACK,
        );
    }

    fn draw_bottom(&self) {
        display::draw_rect_angle(
            self.x_btm,
            OBSTACLE_WIDTH,
            self.y_btm,
            self.height_btm,
            color::BLACK,
        );
    }

    pub fn move_obstacle(&mut self) {
        self.x_top -= self.speed as Coord;
        self.x_btm -= self.speed as Coord;
        self.draw();
        self.clear();

        if self.x_top <= LCD_BIGIN {
            self.x_top = LCD_END;
            self.already_scored = false;
        }

        if self.x_btm <= LCD_BIGIN {
            self.x_btm = LCD_END;
        }
    }

    pub fn get_xy_top(&self) -> (Coord, Coord) {
        (self.x_top, self.y_top)
    }

    pub fn get_xy_bottom(&self) -> (Coord, Coord) {
        (self.x_btm, self.y_btm)
    }

    pub fn get_height(&self) -> (u32, u32) {
        (self.height_top, self.height_btm)
    }
}
