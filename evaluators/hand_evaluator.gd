extends Control

var ai_util: AiUtil
var suit_buttons: Array[TextureButton]

func _ready() -> void:
	ai_util = AiUtil.new()
	suit_buttons = [$Diamonds, $Clubs, $Hearts, $Spades]

var is_running = -1
var run_count = 0
func _process(delta: float) -> void:
	if is_running >= 0 and run_count < 20:
		if is_running < 4:
			ai_util.run_mcts(100, is_running)
		else:
			ai_util.run_trump_mcts(100)
			for i in range(4):
				var res = int(ai_util.get_trump_average_score(i))
				suit_buttons[i].get_node("Label").text = str((res + 162)/2)
		run_count += 1
	if run_count == 20:
		is_running = -1

func set_trump(new_trump: int) -> void:
	is_running = new_trump
	ai_util.init()
	launch_computation()

func _on_run_pressed() -> void:
	is_running = 4
	ai_util.init_for_trumps()
	launch_computation()

func launch_computation():
	run_count = 0
	ai_util.clear_hand()
	for card in $Control/Hand.cards:
		var cpp_rank = 7 if card.rank == 1 else (card.rank - 7)
		ai_util.add_card(card.suit, cpp_rank)
