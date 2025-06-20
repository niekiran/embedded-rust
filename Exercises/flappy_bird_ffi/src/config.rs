pub type Coord = i32;

pub const LCD_WIDTH: u32 = 240;
pub const LCD_HEIGHT: u32 = 240;

pub const OBSTACLE_WIDTH: u32 = 30;
pub const OBSTACLE_GAP: u32 = 80;

pub const SCORE_BOARD_HEIGHT: u32 = 30;
pub const PLANTS_HEIGHT: u32 = 30;

pub const LCD_BIGIN: Coord = 0;
pub const LCD_END: Coord = LCD_WIDTH as Coord;

pub const INIT_PLAYER_POS_X: Coord = 60;
pub const INIT_PLAYER_POS_Y: Coord = (SCORE_BOARD_HEIGHT + 10) as Coord;
pub const PLAYER_WIDTH: u32 = 30;
pub const PLAYER_HEIGHT: u32 = 30;

pub const GRAVITY: i32 = 0;

pub const GROUND_Y_POS: Coord = 210;


pub const MPU6050_DEV_ADDR: u8 = 0x68;

pub const PLAYER_Y_MIN: Coord = SCORE_BOARD_HEIGHT as Coord;
pub const PLAYER_Y_MAX: Coord = (LCD_HEIGHT - PLANTS_HEIGHT - PLAYER_HEIGHT) as Coord;

pub const SPEED: u32 = 2;