extends Node2D

signal no_trump
signal player_chose(player: int, card: Card, trump: Card.Suit)

var start_position = 0
var current_position = 0
var current_card: Card = null

var trump = null

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	$Placeholder.visible = false
	rotation = start_position * PI / 2

func add_card(card):
	ChildExchange.exchange(card, self)
	current_card = card
	[$Diamonds, $Clubs, $Hearts, $Spades][current_card.suit].disabled = true
	[$Diamonds, $Clubs, $Hearts, $Spades][current_card.suit].modulate.a = 0.25
	get_tree().create_timer(1).timeout.connect(func(): card_taken(current_card.suit))
	process_card()


func process_card():
	current_card.tween_position($Placeholder.position, $Placeholder.rotation, 0.5)
	current_card.flip()

func _on_pick_pressed() -> void:
	card_taken(current_card.suit)

func _on_refuse_pressed() -> void:
	current_position += 1
	if current_position == 4:
		for suit_button in [$Diamonds, $Clubs, $Hearts, $Spades]:
			suit_button.visible = true
		$Pick.visible = false
		$Refuse.visible = false
	if current_position == 8:
		no_trump.emit()
	else:
		var tween = create_tween()
		tween.tween_property(self, "rotation_degrees", (start_position + current_position) * 90, 0.5)

func card_taken(suit: Card.Suit):
	player_chose.emit((start_position + current_position) % 4, current_card, suit)
func _on_spades_pressed() -> void:
	card_taken(Card.Suit.SPADES)
func _on_hearts_pressed() -> void:
	card_taken(Card.Suit.HEARTS)
func _on_clubs_pressed() -> void:
	card_taken(Card.Suit.CLUBS)
func _on_diamonds_pressed() -> void:
	card_taken(Card.Suit.DIAMONDS)
