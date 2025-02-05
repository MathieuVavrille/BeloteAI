extends Node2D

@export var small_radius = 100
@export var big_radius = 200
@export var natural_order = true

const CARD_SCENE = preload("res://card.tscn")
var min_angle = PI / 5
var max_angle = PI - min_angle
var angle_between_cards = PI / 12

var cards = []
var cards_goal_angles = []
var ANGLE_SPEED = PI / 2

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass

func add_card(card):
	cards.append(card)
	cards.sort_custom(Card.generate_order(null))
	compute_goal_angles()

func create_card(suit, rank):
	var card = CARD_SCENE.instantiate()
	card.suit = suit
	card.rank = rank
	card.mouse_entered.connect(on_card_mouse_hovered)
	card.mouse_exited.connect(on_card_mouse_hovered)
	add_child(card)
	add_card(card)
	card.angle = PI/2
	

func position_cards():
	for i in range(len(cards)):
		var angle = cards[i].angle
		cards[i].position = Vector2(cos(angle) * big_radius, -sin(angle) * small_radius)
		var card_rotation = -small_radius**2 * cards[i].position.x / big_radius**2 / cards[i].position.y
		cards[i].rotation = atan(card_rotation)
		cards[i].z_index = i

func compute_goal_angles():
	cards_goal_angles = []
	for i in range(len(cards)):
		cards_goal_angles.append(PI/2 - angle_between_cards * (i - (len(cards)-1) / 2.))

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

func on_card_mouse_hovered():
	var can_hover = true
	for i in range(len(cards)-1, -1, -1):
		if cards[i].is_hovered and can_hover:
			cards[i].goal_scale = 1.1
			can_hover = false
		else:
			cards[i].goal_scale = 1.


func _on_spades_pressed() -> void:
	cards.sort_custom(Card.generate_order(Card.Suit.SPADES))
	compute_goal_angles()
func _on_hearts_pressed() -> void:
	cards.sort_custom(Card.generate_order(Card.Suit.HEARTS))
	compute_goal_angles()
func _on_clubs_pressed() -> void:
	cards.sort_custom(Card.generate_order(Card.Suit.CLUBS))
	compute_goal_angles()
func _on_diamonds_pressed() -> void:
	cards.sort_custom(Card.generate_order(Card.Suit.DIAMONDS))
	compute_goal_angles()


func _on_new_card_pressed() -> void:
	var card = CARD_SCENE.instantiate()
	add_child(card)
	card.set_random()
	card.angle = PI/2
	add_card(card)
