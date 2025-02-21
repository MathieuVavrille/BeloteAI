extends Control

var ai_util: AiUtil
var is_running = false

func _ready() -> void:
	ai_util = AiUtil.new()


func _process(delta: float) -> void:
	if is_running:
		ai_util.compute_for_(0.1)

func set_trump(new_trump: int) -> void:
	ai_util.set_trump(new_trump);


func _on_run_pressed() -> void:
	if len($Control/Hand.cards) == 8:
		for card in $Control/Hand.cards:
			var cpp_rank = 7 if card.rank == 1 else (card.rank - 7)
			ai_util.add_card(card.suit, cpp_rank)
	else:
		print("fill the hand")
