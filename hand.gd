extends Node2D

@export var small_radius = 250
@export var big_radius = 400
@export var natural_order = true

const CARD_SCENE = preload("res://card.tscn")
var min_angle = PI / 5
var max_angle = PI - min_angle

var cards = []
var cards_goal_angles = []
var ANGLE_SPEED = PI / 2

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	for i in range(8):
		var card = CARD_SCENE.instantiate()
		card.set_random()
		card.mouse_exited.connect(on_card_mouse_exited)
		card.mouse_entered.connect(on_card_mouse_exited)
		cards.append(card)
		cards[i].angle = max_angle - i * (max_angle - min_angle) / 7
		cards_goal_angles.append(cards[i].angle)
		add_child(card)
	position_cards()

func position_cards():
	for i in range(len(cards)):
		var angle = cards[i].angle
		cards[i].position = Vector2(cos(angle) * big_radius, -sin(angle) * small_radius)
		var card_rotation = -small_radius**2 * cards[i].position.x / big_radius**2 / cards[i].position.y
		cards[i].rotation = atan(card_rotation)
		cards[i].z_index = i

func compute_goal_angles():
	for i in range(len(cards)):
		cards_goal_angles[i] = max_angle - i * (max_angle - min_angle) / (len(cards) - 1)

func move_cards(delta):
	for i in range(len(cards)):
		cards[i].angle = move_toward(cards[i].angle, cards_goal_angles[i], delta * ANGLE_SPEED)

func swap_cards(i, j):
	var cardi = cards[i]
	cards[i] = cards[j]
	cards[j] = cardi

func _process(delta: float) -> void:
	if Input.is_action_just_pressed("ui_accept"):
		pass
		#cards.sort_custom(Card.order)
		#compute_goal_angles()
	move_cards(delta)
	position_cards()

func on_card_mouse_exited():
	var can_hover = true
	for i in range(len(cards)-1, -1, -1):
		if cards[i].is_hovered and can_hover:
			cards[i].goal_scale = 1.1
			can_hover = false
		else:
			cards[i].goal_scale = 1.


func _on_spades_pressed() -> void:
	cards.sort_custom(Card.generate_order(Card.Suit.SPADES, natural_order))
	compute_goal_angles()
func _on_hearts_pressed() -> void:
	cards.sort_custom(Card.generate_order(Card.Suit.HEARTS, natural_order))
	compute_goal_angles()
func _on_clubs_pressed() -> void:
	cards.sort_custom(Card.generate_order(Card.Suit.CLUBS, natural_order))
	compute_goal_angles()
func _on_diamonds_pressed() -> void:
	cards.sort_custom(Card.generate_order(Card.Suit.DIAMONDS, natural_order))
	compute_goal_angles()
