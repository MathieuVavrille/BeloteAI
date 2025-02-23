extends Control

var ai_util: AiUtil
var is_running = false

func _ready() -> void:
	ai_util = AiUtil.new()

var run_count = 0
func _process(delta: float) -> void:
	if is_running and run_count < 10:
		print("Run ", run_count)
		ai_util.run_mcts(1000)
		run_count += 1
	if run_count == 10:
		run_count = 0
		is_running = false
		for card in $Control/Hand.cards:
			var cpp_rank = 7 if card.rank == 1 else (card.rank - 7)
			var res = int(ai_util.get_card_average_score(card.suit, cpp_rank))
			card.set_label((162+res)/2)
		ai_util.print_results()

func set_trump(new_trump: int) -> void:
	ai_util.set_trump(new_trump);


func _on_run_pressed() -> void:
	if len($Control/Hand.cards) == 8:
		for card in $Control/Hand.cards:
			var cpp_rank = 7 if card.rank == 1 else (card.rank - 7)
			ai_util.add_card(card.suit, cpp_rank)
		is_running = true
	else:
		print("fill the hand")
