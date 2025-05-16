use core::ffi;

use crate::assets;
use crate::color;
use crate::config;
use crate::config::Coord;
use crate::display;
use crate::obstacle;
use crate::player;

extern "C" {
    fn HAL_GetTick() -> u32;
}


pub enum GameState {
    Start,
    Running,
    End,
    Halt,
}

pub struct Game {
    state: GameState,
    score: u32,
    countdown_start_time: u32,
    obstacle: obstacle::Obstacle,
    player: player::Player,
}

impl Game {
    pub fn init() -> Self {
        Game {
            state: GameState::Start,
            score: 888,
            countdown_start_time: 0,
            obstacle: obstacle::Obstacle::init(),
            player: player::Player::init(),
            
        }
    }

    pub fn update(&mut self) {
        match self.state {
            GameState::Start => {
                if self.run_countdown() {
                    Game::set_background();
                    self.state = GameState::Running;
                }
            }

            GameState::Running => {
               self.player.move_player();
                self.obstacle.move_obstacle();

                if self.is_collison() {
                    self.state = GameState::End;
                }
            }

            GameState::End => {
                Game::draw_game_over_screen();
                self.show_score(96, 156);
                self.state = GameState::Halt;
            }

            GameState::Halt => {

            }
        }
    }

    pub fn draw_game_over_screen() {
        Game::set_background();
        display::draw_image(40, 160, 40, 80, &assets::GAME_OVER_IMAGE_DATA);
    }

    pub fn draw_start_screen() {
        Game::set_background();
        display::draw_image(40, 160, 40, 80, &assets::GAME_NAME_IMG_DATA);
        let text = c"Game Starts In";
        display::write_string(0, 120, text, color::RED, color::BACKGROUND);
    }

    pub fn set_background() {
        //1. set the background color
        display::set_background_color(color::BACKGROUND);

        //2. print the scoreboard area
        print_score_card_background();

        //3. print the plant
        display::draw_image(0, 60, 210, config::PLANTS_HEIGHT, &assets::PLANT_IMG_DATA);
        display::draw_image(60, 60, 210, config::PLANTS_HEIGHT, &assets::PLANT_IMG_DATA);
        display::draw_image(120, 60, 210, config::PLANTS_HEIGHT, &assets::PLANT_IMG_DATA);
        display::draw_image(180, 60, 210, config::PLANTS_HEIGHT, &assets::PLANT_IMG_DATA);
    }

    //returns 'true' if countdown is over , otherwise 'false'
    fn run_countdown(&mut self) -> bool {
        if self.countdown_start_time == 0 {
            self.countdown_start_time = unsafe { HAL_GetTick() };
        }

        let elapsed = unsafe { HAL_GetTick() } - self.countdown_start_time;
        let number = if elapsed < 1000 {
            c"3"
        } else if elapsed < 2000 {
            c"2"
        } else if elapsed < 3000 {
            c"1"
        } else {
            self.countdown_start_time = 0;
            return true;
        };

        display::write_string(112, 156, number, color::BLACK, color::BACKGROUND);

        false
    }

    fn is_collison(&self) -> bool {
        
        //1. check collison with the ground 
        let (_ , player_y) = self.player.get_xy();
        let hits_ground = 
        (player_y + config::PLAYER_HEIGHT as Coord) >= config::GROUND_Y_POS;
           

        //2. check collision against the obstacles
        let (player_x, player_y) = self.player.get_xy();
        let (top_obstacle_x, top_obstacle_y) = self.obstacle.get_xy_top();
        let (btm_obstacle_x, btm_obstacle_y) = self.obstacle.get_xy_bottom();
        let (top_obstacle_h, _) = self.obstacle.get_height();
       
        let is_horizontal_overlap_with_top =
         ((player_x + config::PLAYER_WIDTH as Coord) > top_obstacle_x) &&
         (player_x < top_obstacle_x + config::OBSTACLE_WIDTH as Coord);

         let is_horizontal_overlap_with_btm =
         ((player_x + config::PLAYER_WIDTH as Coord) > btm_obstacle_x) &&
         (player_x < btm_obstacle_x + config::OBSTACLE_WIDTH as Coord);

        let is_hits_top = player_y <= top_obstacle_y + top_obstacle_h as Coord;
        let is_hits_bottom = (player_y + config::PLAYER_HEIGHT as Coord) >= btm_obstacle_y;

        if hits_ground {
            return true;
        }

        if is_horizontal_overlap_with_top && is_hits_top {
            return true;
        }
    
        if is_horizontal_overlap_with_btm && is_hits_bottom {
            return true;
        }
    
        false

    }

    fn show_score(&self, x: config::Coord, y: config::Coord) {
        let mut buf = [0u8; 4];

        let score = self.score;

        if score >= 1000 {
            buf[0] = b'W';
            buf[1] = b'I';
            buf[2] = b'N';
        } else {
            buf[0] = b'0' + ((score / 100) % 10) as u8;
            buf[1] = b'0' + ((score / 10) % 10) as u8;
            buf[2] = b'0' + (score % 10) as u8;
        }

        buf[3] = b'\0';

        let score_str = ffi::CStr::from_bytes_with_nul(&buf);
            display::write_string(
                x ,
                y,
                score_str.unwrap(),
                color::BLACK,
                color::SCORE,
            );
        
    }


    pub fn is_over(&self) -> bool {

        // match self.state {
        //     GameState::Halt => true, 
        //     _ => false,
        // }
        matches!(self.state, GameState::Halt)
    }
}

fn print_score_card_background() {
    display::draw_rect_angle(0, 240, 0, 28, color::WHITE);
    display::draw_rect_angle(0, 240, 28, 2, color::BLACK);
}
