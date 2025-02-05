extends Node2D
class_name Card

signal mouse_movement

enum Suit { DIAMONDS, CLUBS, HEARTS, SPADES }
const ALL_SUITS = [Suit.DIAMONDS, Suit.CLUBS, Suit.HEARTS, Suit.SPADES]

@export var suit: Suit = Suit.SPADES
@export_range(1, 13) var rank: int = 1

var flip_duration = 0.5

var scale_speed = 1

var angle = 0.

func _ready() -> void:
	load_texture()

func load_texture():
	$Sprites/Front.texture = load("res://assets/cards/" + get_card_name())


func _process(_delta: float) -> void:
	if Input.is_action_just_pressed("ui_accept"):
		flip_card()

func tween_scale(goal_scale):
	var tween = create_tween()
	tween.tween_property(self, "scale", Vector2.ONE * goal_scale, 0.1).set_trans(Tween.TRANS_QUAD)

func tween_position(goal_position, goal_rotation, duration):
	var tween = create_tween()
	tween.set_parallel()
	tween.tween_property(self, "position", goal_position, duration).set_trans(Tween.TRANS_QUAD)
	var rot_tween = create_tween()
	rot_tween.tween_property(self, "rotation", goal_rotation, duration).set_trans(Tween.TRANS_QUAD)
	

func flip_card():
	var flip_sprites = func():
		var back_visible = $Sprites/Back.visible
		$Sprites/Back.visible = $Sprites/Front.visible
		$Sprites/Front.visible = back_visible
	var tween = $Sprites.create_tween()
	tween.tween_property(self, "scale", Vector2(0, 1), flip_duration / 2.).set_ease(Tween.EASE_IN)
	tween.tween_property(self, "scale", Vector2(1, 1), flip_duration / 2.).set_ease(Tween.EASE_OUT)
	get_tree().create_timer(flip_duration / 2.).timeout.connect(flip_sprites)


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
	
	

func set_random():
	suit = ALL_SUITS[randi() % Suit.size()]  # Pick a random suit
	rank = randi() % 13 + 1  # Pick a rank from 1 (Ace) to 13 (King)
	load_texture()

var is_hovered = false
func _on_area_2d_mouse_entered() -> void:
	is_hovered = true
	mouse_movement.emit()
func _on_area_2d_mouse_exited() -> void:
	is_hovered = false
	mouse_movement.emit()

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
