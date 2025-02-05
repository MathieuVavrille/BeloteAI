extends Node2D
class_name Card

signal mouse_entered
signal mouse_exited

enum Suit { DIAMONDS, CLUBS, HEARTS, SPADES }
const ALL_SUITS = [Suit.DIAMONDS, Suit.CLUBS, Suit.HEARTS, Suit.SPADES]

@export var suit: Suit = Suit.SPADES
@export_range(1, 13) var rank: int = 1

var is_flipping = false
var flip_duration = 0.5
var flip_position = -1.

var scale_speed = 1
var goal_scale = 1.

var angle = 0.

func _ready() -> void:
	load_texture()

func load_texture():
	$Front.texture = load("res://assets/cards/" + get_card_name())


func _process(delta: float) -> void:
	if is_flipping:
		if flip_position < 0 and flip_position + PI * delta / flip_duration >= 0:
			var back_visible = $Back.visible
			$Back.visible = $Front.visible
			$Front.visible = back_visible
		flip_position += PI * delta / flip_duration
		$Front.scale.x = abs(sin(flip_position * PI / 2))
		$Back.scale.x = abs(sin(flip_position * PI / 2))
		if flip_position > 1:
			is_flipping = false
			$Front.scale.x = 1
			$Back.scale.x = 1
	scale = scale.move_toward(Vector2.ONE * goal_scale, scale_speed * delta)

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

func flip_card():
	is_flipping = true
	flip_position = -1.

func set_random():
	suit = ALL_SUITS[randi() % Suit.size()]  # Pick a random suit
	rank = randi() % 13 + 1  # Pick a rank from 1 (Ace) to 13 (King)
	load_texture()

var is_hovered = false
func _on_area_2d_mouse_entered() -> void:
	is_hovered = true
	mouse_entered.emit()
func _on_area_2d_mouse_exited() -> void:
	is_hovered = false
	mouse_exited.emit()

static var CARDS_VALUES = [-100, 11, -16, -15, -14, -13, -12, -3, -2, -1, 10, 2, 3, 4]
static var TRUMP_VALUES = [-100, 11, -16, -15, -14, -13, -12, -3, -2, 14, 10, 20, 3, 4]
static func generate_order(trump):
	var order = func(card1, card2):
		if card1.suit != card2.suit:
			return card1.suit < card2.suit
		elif card1.suit == trump:
			return TRUMP_VALUES[card1.rank] < TRUMP_VALUES[card2.rank]
		else:
			return CARDS_VALUES[card1.rank] < CARDS_VALUES[card2.rank]
	return order
