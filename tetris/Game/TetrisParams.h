/*

   TetrisParams

   Problem-specific parameters, stored as globals
   Should be a Params object
*/

#ifndef _H_TETRISPARAMS
#define _H_TETRISPARAMS

#include <iostream.h>

#ifndef FILE
#include <stdio.h>	/* for FILE */
#endif

enum {

param_board_x, param_board_y, param_mem_size, param_mem_max,
param_val_min, param_val_max, param_num_fc, param_num_validate,
param_mod_readboard, param_drop_time,
param_score_piece, param_score_line,
param_score_pixel, param_score_hole,
param_piece_on_board, param_auto_init_player,
param_show_mem, 
param_game_time,
param_inc_drop, param_rover_on_leash, param_ping_pong,
param_smallest_fitness, param_diff_fitness, 
param_rand_start_loc,
param_efd_weight, param_efd_threshold, param_min_num_fc,
param_binary_tournament, param_center_piece, param_specialize_adf,
param_halt_allowed, param_term_roving_sensor,
param_term_uniquad,
param_term_edges,
param_sensor_sees_edges,

param_fitness_queue,  // only because I'm too lazy to put it in a new
                      // ProbParams object
param_catchup,        // likewise
paramMax  /* THIS ONE MUST BE LAST! */
};

#ifdef MACINTOSH
extern int gParams[paramMax];
#else
extern int gParams[];
#endif

#ifdef __cplusplus
extern "C"  {
#endif

void DefaultParams(void);
int LookupParamName(char *sz);
int ReadParam(char *name, char *val);
int WriteParams(FILE *f);

#ifdef __cplusplus
};
#endif

int WriteTetrisParams(ostream&);

#endif  /* _H_PARAMS */


/* EOF */
