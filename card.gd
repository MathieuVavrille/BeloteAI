extends Node2D

enum Suit { SPADES, HEARTS, DIAMONDS, CLUBS }

@export var suit: Suit = Suit.SPADES
@export_range(1, 13) var rank: int = 1

func _ready() -> void:
	print(get_card_name)
	$Front.texture = load("res://assets/cards/" + get_card_name())

func _process(delta: float) -> void:
	if Input.is_action_just_pressed("ui_accept")


func get_card_name() -> String:
	var rank_str: String
	match rank:
		1: rank_str = "A"
		10: rank_str = "10"
		11: rank_str = "J"
		12: rank_str = "Q"
		13: rank_str = "K"
		_: rank_str = "0" + str(rank)  # Default case for numbers 2-10
	var suit_str: String
	match suit:
		Suit.HEARTS: suit_str = "hearts"
		Suit.DIAMONDS: suit_str = "diamonds"
		Suit.CLUBS: suit_str = "clubs"
		Suit.SPADES: suit_str = "spades"
		_: suit_str = "unknown"

	return "card_%s_%s.png" % [suit_str, rank_str]
